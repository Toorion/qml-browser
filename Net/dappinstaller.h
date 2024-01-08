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
#ifndef DAPPINSTALLER_H
#define DAPPINSTALLER_H

#include <QObject>
#include <QUrl>
#include "git2.h"
#include <QtConcurrent>

class DappInstaller : public QObject
{
    Q_OBJECT
public:
    explicit DappInstaller(QObject *parent = nullptr);

    static DappInstaller &instance();

    const std::string getLatestBranch(const QList<QString> tags);

    const QString installOrUpdate(const QUrl &url, const QString &targetDir);

    bool isRepository(const QString &targetDir);

    bool fetch(const QString &targetDir, const QString remoteName);

    bool clone(const QUrl &url, const char *path);

    bool status(const char *path);

    bool checkout(const char *path, const std::string tag);

    bool mergeToHead(const char *path);

    static int findMaster(const char *ref_name, const char *remote_url, const git_oid *oid, unsigned int is_merge, void *payload);

    static int fetchProgress(const git_indexer_progress *stats, void *payload);
    static int sidebandProgress(const char *str, int len, void *payload);
    static void checkoutProgress(const char *path, size_t cur, size_t tot, void *payload);
    QList<QString> remoteLs(char *url);
    QList<QString> localLs(char *path);

    bool proecessIsActive() {
        return m_processIsActive;
    }

    static int credAcquireCb(git_credential **out,
                        const char *url,
                        const char *username_from_url,
                        unsigned int allowed_types,
                        void *payload);

    static int guessRefish(git_annotated_commit **out, git_repository *repo, const char *ref);

    int resolveRefish(git_annotated_commit **commit, git_repository *repo, const char *refish);

    static int performFastforward(git_repository *repo, const git_oid *target_oid, int is_unborn);

signals:

    void progressChanged(int progress, QString status);
    void progressOutput(QString message);
    void finished();

private:
    QUrl m_url;

    QFutureWatcher<void> m_watcher;

    static QString m_error;

    static git_oid m_git_oid;

    bool m_processIsActive = false;

    static QString m_username;

    static QString m_password;

};

#endif // DAPPINSTALLER_H
