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
#ifndef NETWORKACCESSMANAGERFACTORY_H
#define NETWORKACCESSMANAGERFACTORY_H

#include "Net_global.h"
#include <QQmlNetworkAccessManagerFactory>
#include "networkdiskcache.h"

class NET_EXPORT NetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:

    QNetworkAccessManager *create(QObject *parent) override;

    void setMaxCacheSize(qint64 size);

    void enableCache(QString cacheDir, QObject *parent);

    void setUserAgent(const QString userAgent);

    void setReloading(const bool reloading) {
        m_reloading = reloading;
    }

    bool reloading() {
        return m_reloading;
    }

private:

    QObject *m_parent;

    static qint64 m_maxCacheSize;

    QString m_userAgent;

    bool m_reloading = false;

    NetworkDiskCache *m_cache = nullptr;
};

#endif // NETWORKACCESSMANAGERFACTORY_H
