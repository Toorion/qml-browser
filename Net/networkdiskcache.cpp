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
*
* This library baset on QT5 QNetworkDiskCache source code
****************************************************************************/

// #define NETWORKDISKCACHE_DEBUG

#include "networkdiskcache.h"
#include "networkdiskcache_p.h"
#include "QtCore/qscopedpointer.h"
#include <qfile.h>
#include <qdir.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qdiriterator.h>
#include <qurl.h>
#include <qcryptographichash.h>
#include <qdebug.h>
#include "apppaths.h"

#define CACHE_POSTFIX QLatin1String("d")
#define PREPARED_SLASH QLatin1String("prepared/")
#define CACHE_VERSION 1
#define CACHE_DIR QLatin1String("cache")
#define DATA_DIR QLatin1String("data")

#define MAX_COMPRESSION_SIZE (1024 * 1024 * 3)

NetworkDiskCache *NetworkDiskCache::self = nullptr;


/*!
    Creates a new disk cache. The \a parent argument is passed to
    QAbstractNetworkCache's constructor.
 */
NetworkDiskCache::NetworkDiskCache(QString cacheDir, QObject *parent)
    : QAbstractNetworkCache(*new NetworkDiskCachePrivate, parent),
    m_cacheDir(cacheDir)
{
    self = this;
}

/*!
    Destroys the cache object.  This does not clear the disk cache.
 */
NetworkDiskCache::~NetworkDiskCache()
{
    Q_D(NetworkDiskCache);
    qDeleteAll(d->inserting);
}

QString NetworkDiskCache::cacheDirectory(const QUrl &url)
{
    QString cacheDirectory = m_cacheDir + QLatin1Char('/') + CACHE_DIR + QLatin1Char('/');
    QDir helper;
    helper.mkpath(cacheDirectory + PREPARED_SLASH);
    return cacheDirectory;
}

QString NetworkDiskCache::dataDirectory(const QUrl &url)
{
    QString cacheDir = cacheDirectory(url) + DATA_DIR + QString::number(CACHE_VERSION) + QLatin1Char('/');
    QDir helper;
    helper.mkpath(cacheDir);
    return cacheDir;
}


/*!
    \reimp
*/
qint64 NetworkDiskCache::cacheSize() const
{
    return 0; // todo check cache size per application
}

/*!
    \reimp
*/
QIODevice *NetworkDiskCache::prepare(const QNetworkCacheMetaData &metaData)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::prepare()" << metaData.url();
#endif
    Q_D(NetworkDiskCache);
    if (!metaData.isValid() || !metaData.url().isValid() || !metaData.saveToDisk())
        return nullptr;

    const auto headers = metaData.rawHeaders();
    for (const auto &header : headers) {
        if (header.first.compare("content-length", Qt::CaseInsensitive) == 0) {
            const qint64 size = header.second.toLongLong();
            if (size > (maximumCacheSize() * 3)/4)
                return nullptr;
            break;
        }
    }

    std::unique_ptr<CacheItem> cacheItem = std::make_unique<CacheItem>();
    cacheItem->metaData = metaData;

    QIODevice *device = nullptr;
    if (cacheItem->canCompress()) {
        cacheItem->data.open(QBuffer::ReadWrite);
        device = &(cacheItem->data);
    } else {
        QString headerTplName = d->tmpHeaderFileName(cacheItem->metaData.url());
        QString templateName = d->tmpCacheFileName(metaData.url());
        QT_TRY {
            cacheItem->headerFile = new QTemporaryFile(headerTplName, &cacheItem->headerData);
            cacheItem->file = new QTemporaryFile(templateName, &cacheItem->data);
        } QT_CATCH(...) {
            cacheItem->headerFile = nullptr;
            cacheItem->file = nullptr;
        }
        if (!cacheItem->headerFile || !cacheItem->headerFile->open()) {
            qWarning("NetworkDiskCache::prepare() unable to open temporary file");
            cacheItem.reset();
            return nullptr;
        }
        if (!cacheItem->file || !cacheItem->file->open()) {
            qWarning("NetworkDiskCache::prepare() unable to open temporary file");
            cacheItem.reset();
            return nullptr;
        }

        cacheItem->writeHeader(cacheItem->headerFile);
        device = cacheItem->file;
    }
    d->inserting[device] = cacheItem.release();
    return device;
}

/*!
    \reimp
*/
void NetworkDiskCache::insert(QIODevice *device)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::insert()" << device;
#endif
    Q_D(NetworkDiskCache);
    const auto it = d->inserting.constFind(device);
    if (Q_UNLIKELY(it == d->inserting.cend())) {
        qWarning() << "NetworkDiskCache::insert() called on a device we don't know about" << device;
        return;
    }

    d->storeItem(it.value());
    delete it.value();
    d->inserting.erase(it);
}

void NetworkDiskCachePrivate::storeItem(CacheItem *cacheItem)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCachePrivate::storeItem()" << cacheItem->metaData.url();
#endif

//    Q_Q(NetworkDiskCache);
    Q_ASSERT(cacheItem->metaData.saveToDisk());

    QString headerFileName = cacheHeaderFileName(cacheItem->metaData.url());
    Q_ASSERT(!headerFileName.isEmpty());
    if (QFile::exists(headerFileName)) {
        if (!QFile::remove(headerFileName)) {
            qWarning() << "NetworkDiskCache: couldn't remove the cache header file " << headerFileName;
            return;
        }
    }

    QString fileName = cacheFileName(cacheItem->metaData.url());
    Q_ASSERT(!fileName.isEmpty());
    if (QFile::exists(fileName)) {
        if (!QFile::remove(fileName)) {
            qWarning() << "NetworkDiskCache: couldn't remove the cache file " << fileName;
            return;
        }
    }

//    if (currentCacheSize > 0)
//        currentCacheSize += 1024 + cacheItem->size();
//    currentCacheSize = q->expire();

    if (!cacheItem->headerFile) { // Only if canCompress
        cacheItem->headerFile = new QTemporaryFile(tmpHeaderFileName(cacheItem->metaData.url()), &cacheItem->data);
        if (cacheItem->headerFile->open()) {
            cacheItem->writeHeader(cacheItem->headerFile);
            cacheItem->writeCompressedData(cacheItem->headerFile);
        }
    }

    if (cacheItem->headerFile
        && cacheItem->headerFile->isOpen()
        && cacheItem->headerFile->error() == QFile::NoError) {
        cacheItem->headerFile->setAutoRemove(false);
        // ### use atomic rename rather then remove & rename
        if (cacheItem->headerFile->rename(headerFileName))
//            currentCacheSize += cacheItem->headerFile->size();
            ;
        else
            cacheItem->headerFile->setAutoRemove(true);
    }
    if (cacheItem->file
        && cacheItem->file->isOpen()
        && cacheItem->file->error() == QFile::NoError) {
        cacheItem->file->setAutoRemove(false);
        // ### use atomic rename rather then remove & rename
        if (cacheItem->file->rename(fileName))
//            currentCacheSize += cacheItem->file->size();
            ;
        else
            cacheItem->file->setAutoRemove(true);
    }
    if (cacheItem->metaData.url() == lastItem.metaData.url())
        lastItem.reset();
}

/*!
    \reimp
*/
bool NetworkDiskCache::remove(const QUrl &url)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::remove()" << url;
#endif
    Q_D(NetworkDiskCache);

    // remove is also used to cancel insertions, not a common operation
    for (auto it = d->inserting.cbegin(), end = d->inserting.cend(); it != end; ++it) {
        CacheItem *item = it.value();
        if (item && item->metaData.url() == url) {
            delete item;
            d->inserting.erase(it);
            return true;
        }
    }

    if (d->lastItem.metaData.url() == url)
        d->lastItem.reset();
    return d->removeFile(d->cacheFileName(url)) && d->removeFile(d->cacheHeaderFileName(url));
}

/*!
    Put all of the misc file removing into one function to be extra safe
 */
bool NetworkDiskCachePrivate::removeFile(const QString &file)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::removFile()" << file;
#endif
    if (file.isEmpty())
        return false;
    QFileInfo info(file);
    QString fileName = info.fileName();
//    qint64 size = info.size();
    if (QFile::remove(file)) {
//        currentCacheSize -= size;
        return true;
    }
    return false;
}

/*!
    \reimp
*/
QNetworkCacheMetaData NetworkDiskCache::metaData(const QUrl &url)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::metaData()" << url;
#endif
    Q_D(NetworkDiskCache);
    if (d->lastItem.metaData.url() == url)
        return d->lastItem.metaData;

    QString headerFileName = d->cacheHeaderFileName(url);
    return fileMetaData(d->cacheHeaderFileName(url));
}

/*!
    Returns the QNetworkCacheMetaData for the cache file \a fileName.

    If \a fileName is not a cache file QNetworkCacheMetaData will be invalid.
 */
QNetworkCacheMetaData NetworkDiskCache::fileMetaData(const QString &fileName) const
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::fileMetaData()" << fileName;
#endif
    Q_D(const NetworkDiskCache);
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return QNetworkCacheMetaData();
    if (!d->lastItem.read(&file, false)) {
        file.close();
        NetworkDiskCachePrivate *that = const_cast<NetworkDiskCachePrivate*>(d);
        that->removeFile(fileName);
    }
    return d->lastItem.metaData;
}

QString NetworkDiskCache::filePath(const QUrl &url)
{
    Q_D(NetworkDiskCache);
    return d->cacheFileName(url);
}

/*!
    \reimp
*/
QIODevice *NetworkDiskCache::data(const QUrl &url)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::data()" << url;
#endif
    Q_D(NetworkDiskCache);
    std::unique_ptr<QBuffer> buffer;
    if (!url.isValid())
        return nullptr;
    if (d->lastItem.metaData.url() == url && d->lastItem.data.isOpen()) {
        buffer.reset(new QBuffer);
        buffer->setData(d->lastItem.data.data());
    } else {
        QScopedPointer<QFile> headerFile(new QFile(d->cacheHeaderFileName(url)));
        if (!headerFile->open(QFile::ReadOnly | QIODevice::Unbuffered))
            return nullptr;

        if (!d->lastItem.read(headerFile.data(), true)) {
            headerFile->close();
            remove(url);
            return nullptr;
        }
        if (d->lastItem.data.isOpen()) {
            // compressed
            buffer.reset(new QBuffer);
            buffer->setData(d->lastItem.data.data());
        } else {
            QScopedPointer<QFile> file(new QFile(d->cacheFileName(url)));
            if (!file->open(QFile::ReadOnly | QIODevice::Unbuffered))
                return nullptr;

            buffer.reset(new QBuffer);
            buffer->setData(file->readAll());
        }
    }
    buffer->open(QBuffer::ReadOnly);
    return buffer.release();
}

/*!
    \reimp
    For HTTP 304 - Not Modified
*/
void NetworkDiskCache::updateMetaData(const QNetworkCacheMetaData &metaData)
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug() << "NetworkDiskCache::updateMetaData()" << metaData.url();
#endif
    QUrl url = metaData.url();
    QIODevice *oldDevice = data(url);
    if (!oldDevice) {
#if defined(NETWORKDISKCACHE_DEBUG)
        qDebug("NetworkDiskCache::updateMetaData(), no device!");
#endif
        return;
    }

    QIODevice *newDevice = prepare(metaData);
    if (!newDevice) {
#if defined(NETWORKDISKCACHE_DEBUG)
        qDebug() << "NetworkDiskCache::updateMetaData(), no new device!" << url;
#endif
        return;
    }
    char data[1024];
    while (!oldDevice->atEnd()) {
        qint64 s = oldDevice->read(data, 1024);
        newDevice->write(data, s);
    }
    delete oldDevice;
    insert(newDevice);
}

/*!
    Returns the current maximum size for the disk cache.

    \sa setMaximumCacheSize()
 */
qint64 NetworkDiskCache::maximumCacheSize() const
{
    Q_D(const NetworkDiskCache);
    return d->maximumCacheSize;
}

/*!
    Sets the maximum size of the disk cache to be \a size.

    \sa maximumCacheSize()
 */
void NetworkDiskCache::setMaximumCacheSize(qint64 size)
{
    Q_D(NetworkDiskCache);
    d->maximumCacheSize = size;
}

/*!
    Cleans the cache so that its size is under the maximum cache size.
    Returns the current size of the cache.

    When the current size of the cache is greater than the maximumCacheSize()
    older cache files are removed until the total size is less then 90% of
    maximumCacheSize() starting with the oldest ones first using the file
    creation date to determine how old a cache file is.

    Subclasses can reimplement this function to change the order that cache
    files are removed taking into account information in the application
    knows about that NetworkDiskCache does not, for example the number of times
    a cache is accessed.

    \note cacheSize() calls expire if the current cache size is unknown.

    \sa maximumCacheSize(), fileMetaData()
 */
/*
qint64 NetworkDiskCache::expire()
{
    Q_D(NetworkDiskCache);
    if (d->currentCacheSize >= 0 && d->currentCacheSize < maximumCacheSize())
        return d->currentCacheSize;

    if (cacheDirectory().isEmpty()) {
        qWarning("NetworkDiskCache::expire() The cache directory is not set");
        return 0;
    }

    // close file handle to prevent "in use" error when QFile::remove() is called
    d->lastItem.reset();

    QDir::Filters filters = QDir::AllDirs | QDir:: Files | QDir::NoDotAndDotDot;
    QDirIterator it(cacheDirectory(), filters, QDirIterator::Subdirectories);

    QMultiMap<QDateTime, QString> cacheItems;
    qint64 totalSize = 0;
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo info = it.fileInfo();
        QString fileName = info.fileName();
        const QDateTime birthTime = info.fileTime(QFile::FileBirthTime);
        cacheItems.insert(birthTime.isValid() ? birthTime
                          : info.fileTime(QFile::FileMetadataChangeTime), path);
        totalSize += info.size();
    }

    qint64 goal = (maximumCacheSize() * 9) / 10;
    QMultiMap<QDateTime, QString>::const_iterator i = cacheItems.constBegin();
    while (i != cacheItems.constEnd()) {
        if (totalSize < goal)
            break;
        QString name = i.value();
        QFile file(name);

        if (name.contains(PREPARED_SLASH)) {
            for (CacheItem *item : qAsConst(d->inserting)) {
                if (item && item->file && item->file->fileName() == name) {
                    delete item->file;
                    item->file = nullptr;
                    break;
                }
            }
        }

        qint64 size = file.size();
        file.remove();
        totalSize -= size;
        ++i;
    }
#if defined(NETWORKDISKCACHE_DEBUG)
    if (removedFiles > 0) {
        qDebug() << "NetworkDiskCache::expire()"
                << "Removed:" << removedFiles
                << "Kept:" << cacheItems.count() - removedFiles;
    }
#endif
    return totalSize;
}
*/

/*!
    \reimp
*/
void NetworkDiskCache::clear()
{
#if defined(NETWORKDISKCACHE_DEBUG)
    qDebug("NetworkDiskCache::clear()");
#endif
    // todo Make clean per each site
    Q_D(NetworkDiskCache);
    qint64 size = d->maximumCacheSize;
    d->maximumCacheSize = 0;
    d->maximumCacheSize = size;
}

/*!
    Given a URL, generates a unique enough filename (and subdirectory)
 */
QString NetworkDiskCachePrivate::uniqueFileName(const QUrl &url, const QString &postfix)
{
    QUrl cleanUrl = url;
    cleanUrl.setPassword(QString());
    cleanUrl.setFragment(QString());

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(cleanUrl.toEncoded());
    // convert sha1 to base36 form and return first 8 bytes for use as string
    const QByteArray id = QByteArray::number(*(qlonglong*)hash.result().constData(), 36).left(8);
    return QLatin1String(id) + QLatin1Char('.') + postfix;
}


QString NetworkDiskCachePrivate::tmpHeaderFileName(const QUrl &url)
{
    Q_Q(NetworkDiskCache);
    //The subdirectory is presumed to be already read for use.
    return q->cacheDirectory(url) + PREPARED_SLASH + QLatin1String("XXXXXX.") + CACHE_POSTFIX;
}


QString NetworkDiskCachePrivate::tmpCacheFileName(const QUrl &url)
{
    Q_Q(NetworkDiskCache);
    QFileInfo info(url.fileName());
    //The subdirectory is presumed to be already read for use.
    return q->cacheDirectory(url) + PREPARED_SLASH + QLatin1String("XXXXXX") + QLatin1Char('.') +
            (info.suffix().isEmpty() ? QLatin1String("z") : info.suffix());
}

/*!
    Generates fully qualified path of cached resource from a URL.
 */
QString NetworkDiskCachePrivate::cacheHeaderFileName(const QUrl &url)
{
    Q_Q(NetworkDiskCache);

    if (!url.isValid())
        return QString();

    return q->dataDirectory(url) + uniqueFileName(url, CACHE_POSTFIX);
}


/*!
    Generates fully qualified path of cached resource from a URL.
 */
QString NetworkDiskCachePrivate::cacheFileName(const QUrl &url)
{
    Q_Q(NetworkDiskCache);
    if (!url.isValid())
        return QString();

    QFileInfo info(url.fileName());

    return q->dataDirectory(url) + uniqueFileName(url, info.suffix());
}

/*!
    We compress small text and JavaScript files.
 */
bool CacheItem::canCompress() const
{
    bool sizeOk = false;
    bool typeOk = false;
    const auto headers = metaData.rawHeaders();
    for (const auto &header : headers) {
        if (header.first.compare("content-length", Qt::CaseInsensitive) == 0) {
            qint64 size = header.second.toLongLong();
            if (size > MAX_COMPRESSION_SIZE)
                return false;
            else
                sizeOk = true;
        }

        if (header.first.compare("content-type", Qt::CaseInsensitive) == 0) {
            QByteArray type = header.second;
            if (type.startsWith("text/")
                    || (type.startsWith("application/")
                        && (type.endsWith("javascript") || type.endsWith("ecmascript"))))
                typeOk = true;
            else
                return false;
        }
        if (sizeOk && typeOk)
            return true;
    }
    return false;
}

enum
{
    CacheMagic = 0xe8,
    CurrentCacheVersion = CACHE_VERSION
};

void CacheItem::writeHeader(QFile *device) const
{
    QDataStream out(device);

    out << qint32(CacheMagic);
    out << qint32(CurrentCacheVersion);
    out << static_cast<qint32>(out.version());
    out << metaData;
    bool compressed = canCompress();
    out << compressed;
}

void CacheItem::write(QFile *device) const
{
    QDataStream out(device);

    out << data.data();
}

void CacheItem::writeCompressedData(QFile *device) const
{
    QDataStream out(device);

    out << qCompress(data.data());
}

/*!
    Returns \c false if the file is a cache file,
    but is an older version and should be removed otherwise true.
 */
bool CacheItem::read(QFile *device, bool readData)
{
    reset();

    QDataStream in(device);

    qint32 marker;
    qint32 v;
    in >> marker;
    in >> v;
    if (marker != CacheMagic)
        return true;

    // If the cache magic is correct, but the version is not we should remove it
    if (v != CurrentCacheVersion)
        return false;

    qint32 streamVersion;
    in >> streamVersion;
    // Default stream version is also the highest we can handle
    if (streamVersion > in.version())
        return false;
    in.setVersion(streamVersion);

    bool compressed;
    QByteArray dataBA;
    in >> metaData;
    in >> compressed;
    if (readData && compressed) {
        in >> dataBA;
        data.setData(qUncompress(dataBA));
        data.open(QBuffer::ReadOnly);
    }

    // quick and dirty check if metadata's URL field and the file's name are in synch
    QString expectedFilename = NetworkDiskCachePrivate::uniqueFileName(metaData.url(), CACHE_POSTFIX);
    if (!device->fileName().endsWith(expectedFilename))
        return false;

    return metaData.isValid();
}

QT_END_NAMESPACE
