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

#ifndef NETWORKDISKCACHE_P_H
#define NETWORKDISKCACHE_P_H

#include "networkdiskcache.h"
#include <QBuffer>
#include <QTemporaryFile>
#include "private/qabstractnetworkcache_p.h"
#include <QHash>

class CacheItem
{
public:
    CacheItem() : file(nullptr)
    {
    }
    ~CacheItem()
    {
        reset();
    }

    QNetworkCacheMetaData metaData;
    QBuffer data;
    QBuffer headerData;
    QTemporaryFile *file;
    QTemporaryFile *headerFile = nullptr;
    inline qint64 size() const
        { return (file ? file->size() : data.size()) + (headerFile ? headerFile->size() : headerData.size()); }

    inline void reset() {
        metaData = QNetworkCacheMetaData();
        headerData.close();
        data.close();
        headerData.close();
        if(headerFile) {
            delete headerFile;
            headerFile = nullptr;
        }
        delete file;
        file = nullptr;
    }
    void writeHeader(QFile *device) const;
    void write(QFile *device) const;
    void writeCompressedData(QFile *device) const;
    bool read(QFile *device, bool readData);

    bool canCompress() const;
};


class NetworkDiskCachePrivate : public QAbstractNetworkCachePrivate
{
public:
    NetworkDiskCachePrivate()
        : QAbstractNetworkCachePrivate()
        , maximumCacheSize(1024 * 1024 * 50)
        , currentCacheSize(-1)
        {}

    static QString uniqueFileName(const QUrl &url, const QString &extension);
    QString cacheHeaderFileName(const QUrl &url) const;
    QString cacheFileName(const QUrl &url) const;
    QString tmpHeaderFileName() const;
    QString tmpCacheFileName(const QString &file) const;
    bool removeFile(const QString &file);
    void storeItem(CacheItem *item);
    void prepareLayout();
    static quint32 crc32(const char *data, uint len);

    mutable CacheItem lastItem;
    QString cacheDirectory;
    QString dataDirectory;
    qint64 maximumCacheSize;
    qint64 currentCacheSize;

    QHash<QIODevice*, CacheItem*> inserting;
    Q_DECLARE_PUBLIC(NetworkDiskCache)
};



#endif // NETWORKDISKCACHE_P_H
