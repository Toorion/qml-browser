/****************************************************************************
*
* QmlBrowser - Web browser with QML page support
* Copyright (C) 2022 Denis Solomatin <toorion@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include "dappinstaller.h"
#include "semver.h"
#include "mainapplication.h"

git_oid DappInstaller::m_git_oid;
QString DappInstaller::m_username;
QString DappInstaller::m_password;
QString DappInstaller::m_error;


DappInstaller::DappInstaller(QObject *parent)
    : QObject{parent}
{

}

DappInstaller &DappInstaller::instance()
{
    static DappInstaller instance;
    return instance;
}

const std::string DappInstaller::getLatestBranch(const QList<QString> tags)
{
    Semver sv;
    for (qsizetype i = 0; i < tags.size(); ++i) {
        auto tag = tags.at(i);
        QString type = tag.section('/', 1, 1);
        if(type.compare("tags", Qt::CaseInsensitive) != 0) {
            continue;
        }
        QString v = tag.section('/', 2, 2);
        if(sv.isEmpty || sv < Semver(v.toLocal8Bit().data())) {
            sv = Semver(v.toLocal8Bit().data());
        }
    }

    if(sv.isEmpty) {
        return {};
    }

    return sv.name;
}

const QString DappInstaller::installOrUpdate(const QUrl &url, const QString &targetDir)
{
    m_processIsActive = true;
    m_username = {};
    m_password = {};
    m_error = {};

    QList<QString> tags = remoteLs(url.toString().toLocal8Bit().data());
    if(!m_error.isEmpty()) {
        m_processIsActive = false;
        return {};
    }
    std::string tag = getLatestBranch(tags);

    QFuture<void> future = QtConcurrent::run([&, url, targetDir, tag]{
        Q_ASSERT(QThread::currentThread() != qApp->thread());

        bool ok = false;
        if(isRepository(targetDir)) {
            // If repo has no tags - update to last commit
            if(tag.empty()) {
                emit progressChanged(0, "Updating");
                ok = fetch(targetDir, "origin");
                if(ok) {
                    ok = mergeToHead(targetDir.toLocal8Bit().data());
                }
            } else { // Check for last tag and if have new update to last version
                QList<QString> localTags = localLs(targetDir.toLocal8Bit().data());
                std::string localTag = getLatestBranch(localTags);
                if(tag.compare(localTag) != 0) {
                    emit progressChanged(0, "Updating");
                    ok = fetch(targetDir, "origin");
                } else {
                    emit progressOutput("No updates found");
                }
            }
        } else {
            emit progressChanged(0, "Begin installation");
            ok = clone(url, targetDir.toLocal8Bit().data());
        }

        if(ok && tag.length() > 0) {
            emit progressOutput(tr("Swithing to tag %1").arg(QString::fromStdString(tag)));
            checkout(targetDir.toLocal8Bit().data(), tag);
        }

        if(ok) {
            emit progressChanged(100, "Installation done!");
            emit progressOutput("Installation done!");
        }
        m_processIsActive = false;
    });

    connect(&m_watcher, &QFutureWatcher<void>::finished, this, &DappInstaller::finished);
    m_watcher.setFuture(future);

    return QString::fromStdString(tag);
}

bool DappInstaller::isRepository(const QString &targetDir)
{
    QDir pathDir(targetDir);
    if (!pathDir.exists())
    {
        return false;
    }
    git_repository *repo = nullptr;
    int err = git_repository_open(&repo, targetDir.toLocal8Bit().data());
    if (err < 0) {
        return false;
    }

    auto state = git_repository_state(repo);

    git_repository_free(repo);

    return GIT_REPOSITORY_STATE_NONE == state;
}

bool DappInstaller::fetch(const QString &targetDir, const QString remoteName)
{
    git_remote *remote = NULL;
    const git_indexer_progress *stats;
    git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

    git_repository *repo = NULL;
    int err = git_repository_open(&repo, targetDir.toLocal8Bit().data());
    if (err < 0) {
        m_error = git_error_last()->message;
        return false;
    }

    /* Figure out whether it's a named remote or a URL */
    if (git_remote_lookup(&remote, repo, remoteName.toLocal8Bit().data()) < 0)
        if (git_remote_create_anonymous(&remote, repo, remoteName.toLocal8Bit().data()) < 0)
            goto on_error;

    /* Set up the callbacks (only update_tips for now) */
    //fetch_opts.callbacks.update_tips = &update_cb;
    fetch_opts.callbacks.sideband_progress = sidebandProgress;
    fetch_opts.callbacks.transfer_progress = &fetchProgress;
    fetch_opts.callbacks.credentials = credAcquireCb;

    /**
     * Perform the fetch with the configured refspecs from the
     * config. Update the reflog for the updated references with
     * "fetch".
     */
    if (git_remote_fetch(remote, NULL, &fetch_opts, "fetch") < 0)
        goto on_error;

    /**
     * If there are local objects (we got a thin pack), then tell
     * the user how many objects we saved from having to cross the
     * network.
     */
    stats = git_remote_stats(remote);
    if (stats->local_objects > 0) {
        emit progressOutput(
            QString::asprintf("\rReceived %u/%u objects in %zu bytes (used %u local objects)\n",
                              stats->indexed_objects, stats->total_objects, stats->received_bytes, stats->local_objects));
    } else{
        emit progressOutput(
            QString::asprintf("\rReceived %u/%u objects in %zu bytes\n",
                              stats->indexed_objects, stats->total_objects, stats->received_bytes));
    }

    git_remote_free(remote);

    return true;

on_error:
    m_error = git_error_last()->message;
    git_remote_free(remote);
    return false;
}

bool DappInstaller::clone(const QUrl &url, const char *path)
{
    if (git_libgit2_init() < 0) {
        m_error = "Git init error";
        return false;
    }

    git_repository *repo = NULL;
    git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
    git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

    // Set up options
    checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    checkout_opts.progress_cb = checkoutProgress;
    clone_opts.checkout_opts = checkout_opts;
    clone_opts.fetch_opts.callbacks.sideband_progress = sidebandProgress;
    clone_opts.fetch_opts.callbacks.transfer_progress = &fetchProgress;
    clone_opts.fetch_opts.callbacks.credentials = credAcquireCb;

    int err = git_clone(
        &repo,
        url.toString().toLocal8Bit().data(),
        path,
        &clone_opts
        );

    git_repository_free(repo);

    if (err != 0) {
        m_error = git_error_last()->message;
        return false;
    }

    return true;
}

bool DappInstaller::checkout(const char *path, const std::string tag)
{
    git_repository *repo = nullptr;
    int err = git_repository_open(&repo, path);
    if (err < 0) {
        m_error = git_error_last()->message;
        return false;
    }

    git_annotated_commit *annotated_commit = nullptr;
    git_reference *ref = nullptr, *branch = nullptr;
    git_commit *target_commit = NULL;

    std::string name = "refs/tags/" + tag;
    char * tagRef = new char[name.length() + 1];
    strcpy(tagRef, name.c_str());

    git_checkout_options checkout_options = GIT_CHECKOUT_OPTIONS_INIT;
    checkout_options.checkout_strategy = GIT_CHECKOUT_SAFE;
    checkout_options.checkout_strategy = GIT_CHECKOUT_FORCE;

    auto state = git_repository_state(repo);
    if (state != GIT_REPOSITORY_STATE_NONE) {
        m_error = tr("repository is in unexpected state %1").arg(state);
        return false;
    }

    if ((err = resolveRefish(&annotated_commit, repo, tagRef)) < 0 &&
        (err = guessRefish(&annotated_commit, repo, tagRef)) < 0) {
        m_error = tr("failed to resolve %1: %2").arg(tagRef).arg(git_error_last()->message);
        goto cleanup;
    }

    err = git_commit_lookup(&target_commit, repo, git_annotated_commit_id(annotated_commit));
    if (err != 0) {
        m_error = tr("failed to lookup commit: %1").arg(git_error_last()->message);
        goto cleanup;
    }

    // Checkout the target commit
    err = git_checkout_tree(repo, (const git_object *)target_commit, &checkout_options);
    if (err < 0) {
        m_error = git_error_last()->message;
        goto cleanup;
    }

    if (git_annotated_commit_ref(annotated_commit)) {
        const char *target_head;

        if ((err = git_reference_lookup(&ref, repo, git_annotated_commit_ref(annotated_commit))) < 0) {
            m_error = tr("failed to update HEAD reference: %1").arg(git_error_last()->message);
            goto cleanup;
        }

        if (git_reference_is_remote(ref)) {
            if ((err = git_branch_create_from_annotated(&branch, repo, tagRef, annotated_commit, 0)) < 0) {
                m_error = tr("failed to update HEAD reference: %s\n").arg(git_error_last()->message);
                goto cleanup;
            }
            target_head = git_reference_name(branch);
        } else {
            target_head = git_annotated_commit_ref(annotated_commit);
        }

        err = git_repository_set_head(repo, target_head);
    } else {
        err = git_repository_set_head_detached_from_annotated(repo, annotated_commit);
    }

cleanup:
    git_annotated_commit_free(annotated_commit);

    return m_error.isEmpty();
}

bool DappInstaller::mergeToHead(const char *path)
{
    git_repository *repo = NULL;
    int err = git_repository_open(&repo, path);
    if(err != 0) {
        m_error = QLatin1String("Open repository error");
        return -1;
    }

    git_index *index;
    git_merge_analysis_t analysis;
    git_merge_preference_t preference;
    git_reference *branch_ref = NULL;
    git_reference *tracking_ref = NULL;
    git_annotated_commit *our_head, *their_heads[1];


    int state = git_repository_state(repo);
    if (state != GIT_REPOSITORY_STATE_NONE) {
        m_error = tr("repository is in unexpected state %1").arg(state);
        return -1;
    }

    git_repository_fetchhead_foreach(repo, findMaster, NULL);

    git_annotated_commit_from_fetchhead(&their_heads[0], repo, "master", path, &m_git_oid);

    err = git_merge_analysis(&analysis, &preference,
                             repo,
                             (const git_annotated_commit **)their_heads,
                             1);
    if (err != 0) {
        m_error = tr("merge error: %1").arg(git_error_last()->message);
        return -1;
    }

    if (analysis & GIT_MERGE_ANALYSIS_UP_TO_DATE) {
        emit progressOutput("Already up-to-date");
        goto cleanup;
    } else if (analysis & GIT_MERGE_ANALYSIS_UNBORN ||
               (analysis & GIT_MERGE_ANALYSIS_FASTFORWARD &&
                !(preference & GIT_MERGE_PREFERENCE_NO_FASTFORWARD))) {
        const git_oid *target_oid;
        if (analysis & GIT_MERGE_ANALYSIS_UNBORN) {
            emit progressOutput("Unborn");
        } else {
            emit progressOutput("Fast-forward");
        }

        /* Since this is a fast-forward, there can be only one merge head */
        target_oid = git_annotated_commit_id(their_heads[0]);

        return performFastforward(repo, target_oid, (analysis & GIT_MERGE_ANALYSIS_UNBORN));
    } else if (analysis & GIT_MERGE_ANALYSIS_NORMAL) {

        m_error = QLatin1String("Unexpected behavior");
        return -1;
/*
        git_merge_options merge_opts = GIT_MERGE_OPTIONS_INIT;
        git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

        merge_opts.flags = 0;
        merge_opts.file_flags = GIT_MERGE_FILE_STYLE_DIFF3;

        checkout_opts.checkout_strategy = GIT_CHECKOUT_FORCE|GIT_CHECKOUT_ALLOW_CONFLICTS;

        if (preference & GIT_MERGE_PREFERENCE_FASTFORWARD_ONLY) {
            printf("Fast-forward is preferred, but only a merge is possible\n");
            return -1;
        }

        err = git_merge(repo,
                        (const git_annotated_commit **)opts.annotated, opts.annotated_count,
                        &merge_opts, &checkout_opts);
*/
    }

    /* If we get here, we actually performed the merge above */

    git_repository_index(&index, repo);
    // , "failed to get repository index", NULL);

    if (git_index_has_conflicts(index)) {
        m_error = QLatin1String("Index jas conflict");
        return -1;
        /* Handle conflicts */
        // output_conflicts(index);
    } else {
        // create_merge_commit(repo, index, &opts);
        emit progressOutput("Merge made");
    }

cleanup:

    return 0;
}

int DappInstaller::findMaster(const char *ref_name, const char *remote_url, const git_oid *oid, unsigned int is_merge, void *payload)
{
    if (is_merge)
        m_git_oid = *oid;
    return 0;
}

int DappInstaller::fetchProgress(const git_indexer_progress *stats, void *payload)
{
    Q_UNUSED(payload);

    int progress = stats->total_objects > 0 ?
        (50*stats->indexed_objects / stats->total_objects) : 0;

    emit DappInstaller::instance().progressChanged(progress, "Fetching...");

    if (stats->total_objects &&
        stats->received_objects == stats->total_objects) {
        emit DappInstaller::instance().progressChanged(99, "Resolving deltas...");
    }

    return 0;
}

int DappInstaller::sidebandProgress(const char *str, int len, void *payload)
{
    Q_UNUSED(len);
    Q_UNUSED(payload);
    emit DappInstaller::instance().progressOutput(str);
    return 0;
}

void DappInstaller::checkoutProgress(const char *path, size_t cur, size_t tot, void *payload)
{
    int progress = tot > 0
                       ? (int)((50 * cur) / tot)
                       : 0;
    emit DappInstaller::instance().progressChanged(50 + progress, "Checking...");
    emit DappInstaller::instance().progressOutput("File: " + QString(path));
}

QList<QString> DappInstaller::remoteLs(char *url)
{
    QList<QString> list;

    if (git_libgit2_init() < 0) {
        m_error = QLatin1String("Git init error!");
        return list;
    }

    git_repository *repo = NULL;
    git_remote *remote = NULL;
    int error;
    const git_remote_head **refs;
    size_t refs_len, i;
    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;

    git_remote_create(&remote, repo, "origin", url);

    /**
     * Connect to the remote and call the printing function for
     * each of the remote references.
     */
    callbacks.credentials = credAcquireCb;

    error = git_remote_connect(remote, GIT_DIRECTION_FETCH, &callbacks, NULL, NULL);
    if (error < 0) {
        m_error = git_error_last()->message;
        goto cleanup;
    }

    /**
     * Get the list of references on the remote and print out
     * their name next to what they point to.
     */
    if (git_remote_ls(&refs, &refs_len, remote) < 0)
        goto cleanup;

    for (i = 0; i < refs_len; i++) {
        list.append(refs[i]->name);
    }

cleanup:
    git_remote_free(remote);
    return list;
}

QList<QString> DappInstaller::localLs(char *path)
{
    QList<QString> list;
    git_repository *repo = NULL;
    int error = git_repository_open(&repo, path);
    git_commit *commit = NULL;
    git_revparse_single((git_object**)&commit, repo, "HEAD");
    git_strarray tag_list;
    git_tag_list(&tag_list, repo);
    for (size_t i = 0; i < tag_list.count; i++) {
        list.append(tag_list.strings[i]);
    }
    return list;
}

int DappInstaller::credAcquireCb(git_credential **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{
    char *privkey = NULL, *pubkey = NULL;
    int error = 1;

    Q_UNUSED(payload);

    if(m_password.isEmpty()) {
        if(strlen(username_from_url) > 0) {
            m_username = username_from_url;
        }

        if(!mainApp->authDialog(m_username, m_password,
                     tr("Authorization required"),
                     tr("A username and password are being requested by %1").arg(url),
                     nullptr)
            ) {
            goto out;
        }
    }

    /*
     * todo
     * if (allowed_types & GIT_CREDENTIAL_SSH_KEY) {
        int n;

        if ((error = ask(&privkey, "SSH Key:", 0)) < 0 ||
            (error = ask(&password, "Password:", 1)) < 0)
            goto out;

        if ((n = snprintf(NULL, 0, "%s.pub", privkey)) < 0 ||
            (pubkey = malloc(n + 1)) == NULL ||
            (n = snprintf(pubkey, n + 1, "%s.pub", privkey)) < 0)
            goto out;

        error = git_credential_ssh_key_new(out, username, pubkey, privkey, password);
    } else if (allowed_types & GIT_CREDENTIAL_USERPASS_PLAINTEXT) { */

    error = git_credential_userpass_plaintext_new(out, m_username.toLocal8Bit().data(), m_password.toLocal8Bit().data());

out:
    free(privkey);
    free(pubkey);
    return error;

}

int DappInstaller::guessRefish(git_annotated_commit **out, git_repository *repo, const char *ref)
{
    git_strarray remotes = { NULL, 0 };
    git_reference *remote_ref = NULL;
    int error;
    size_t i;

    if ((error = git_remote_list(&remotes, repo)) < 0)
        goto out;

    for (i = 0; i < remotes.count; i++) {
        char *refname = NULL;
        size_t reflen;

        reflen = snprintf(refname, 0, "refs/remotes/%s/%s", remotes.strings[i], ref);
        if ((refname = (char *) malloc(reflen + 1)) == NULL) {
            error = -1;
            goto next;
        }
        snprintf(refname, reflen + 1, "refs/remotes/%s/%s", remotes.strings[i], ref);

        if ((error = git_reference_lookup(&remote_ref, repo, refname)) < 0)
            goto next;

        break;
    next:
        free(refname);
        if (error < 0 && error != GIT_ENOTFOUND)
            break;
    }

    if (!remote_ref) {
        error = GIT_ENOTFOUND;
        goto out;
    }

    if ((error = git_annotated_commit_from_ref(out, repo, remote_ref)) < 0)
        goto out;

out:
    git_reference_free(remote_ref);
    git_strarray_dispose(&remotes);
    return error;
}

int DappInstaller::resolveRefish(git_annotated_commit **commit, git_repository *repo, const char *refish)
{
    git_reference *ref;
    git_object *obj;
    int err = 0;

    assert(commit != NULL);

    err = git_reference_dwim(&ref, repo, refish);
    if (err == GIT_OK) {
        git_annotated_commit_from_ref(commit, repo, ref);
        git_reference_free(ref);
        return 0;
    }

    err = git_revparse_single(&obj, repo, refish);
    if (err == GIT_OK) {
        err = git_annotated_commit_lookup(commit, repo, git_object_id(obj));
        git_object_free(obj);
    }

    return err;
}

int DappInstaller::performFastforward(git_repository *repo, const git_oid *target_oid, int is_unborn)
{
    git_checkout_options ff_checkout_options = GIT_CHECKOUT_OPTIONS_INIT;
    git_reference *target_ref;
    git_reference *new_target_ref;
    git_object *target = NULL;
    int err = 0;

    if (is_unborn) {
        const char *symbolic_ref;
        git_reference *head_ref;

        /* HEAD reference is unborn, lookup manually so we don't try to resolve it */
        err = git_reference_lookup(&head_ref, repo, "HEAD");
        if (err != 0) {
            m_error = tr("failed to lookup HEAD ref");
            return -1;
        }

        /* Grab the reference HEAD should be pointing to */
        symbolic_ref = git_reference_symbolic_target(head_ref);

        /* Create our master reference on the target OID */
        err = git_reference_create(&target_ref, repo, symbolic_ref, target_oid, 0, NULL);
        if (err != 0) {
            m_error = tr("failed to create master reference");
            return -1;
        }

        git_reference_free(head_ref);
    } else {
        /* HEAD exists, just lookup and resolve */
        err = git_repository_head(&target_ref, repo);
        if (err != 0) {
            m_error = tr("failed to get HEAD reference");
            return -1;
        }
    }

    /* Lookup the target object */
    err = git_object_lookup(&target, repo, target_oid, GIT_OBJECT_COMMIT);
    if (err != 0) {
        m_error = tr("failed to lookup OID %1", git_oid_tostr_s(target_oid));
        return -1;
    }

    /* Checkout the result so the workdir is in the expected state */
    ff_checkout_options.checkout_strategy = GIT_CHECKOUT_SAFE;
    err = git_checkout_tree(repo, target, &ff_checkout_options);
    if (err != 0) {
        m_error = tr("failed to checkout HEAD reference");
        return -1;
    }

    /* Move the target reference to the target OID */
    err = git_reference_set_target(&new_target_ref, target_ref, target_oid, NULL);
    if (err != 0) {
        m_error = tr("failed to move HEAD reference");
        return -1;
    }

    git_reference_free(target_ref);
    git_reference_free(new_target_ref);
    git_object_free(target);

    return 0;
}

