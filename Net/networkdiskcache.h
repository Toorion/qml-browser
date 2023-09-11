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

#ifndef NETWORKDISKCACHE_H
#define NETWORKDISKCACHE_H

#define httpDiskCache NetworkDiskCache::instance()

#include <QtNetwork/qtnetworkglobal.h>
#include <QtNetwork/qabstractnetworkcache.h>
#include "Net_global.h"

QT_REQUIRE_CONFIG(networkdiskcache);

class NetworkDiskCachePrivate;
class NET_EXPORT NetworkDiskCache : public QAbstractNetworkCache
{
    Q_OBJECT

public:
    explicit NetworkDiskCache(QObject *parent = nullptr);
    ~NetworkDiskCache();

    static NetworkDiskCache* instance();

    QString cacheDirectory(const QUrl &url);
    QString dataDirectory(const QUrl &url);

    qint64 maximumCacheSize() const;
    void setMaximumCacheSize(qint64 size);

    qint64 cacheSize() const override;
    QNetworkCacheMetaData metaData(const QUrl &url) override;
    void updateMetaData(const QNetworkCacheMetaData &metaData) override;
    QIODevice *data(const QUrl &url) override;
    bool remove(const QUrl &url) override;
    QIODevice *prepare(const QNetworkCacheMetaData &metaData) override;
    void insert(QIODevice *device) override;

    QNetworkCacheMetaData fileMetaData(const QString &fileName) const;

    QString filePath(const QUrl &url);

    static NetworkDiskCache *self;

public Q_SLOTS:
    void clear() override;

protected:
//    virtual qint64 expire();

private:
    Q_DECLARE_PRIVATE(NetworkDiskCache)
    Q_DISABLE_COPY(NetworkDiskCache)
};

#endif // NETWORKDISKCACHE_H
