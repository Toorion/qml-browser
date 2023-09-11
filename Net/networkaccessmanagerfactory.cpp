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
#include "networkaccessmanagerfactory.h"
#include <QNetworkAccessManager>
#include "httpmanager.h"
#include "networkdiskcache.h"

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    auto httpManager = new HttpManager(parent);

    auto cache = new NetworkDiskCache();
    cache->setMaximumCacheSize(m_maxCacheSize);

    httpManager->setCache(cache);
    httpManager->setUserAgent(m_userAgent);
    httpManager->setReloading(m_reloading);

    return httpManager;
}

NetworkAccessManagerFactory *NetworkAccessManagerFactory::instance()
{
    static NetworkAccessManagerFactory instance;
    return &instance;
}

void NetworkAccessManagerFactory::setMaxCacheSize(qint64 size)
{
    m_maxCacheSize = size;
}

void NetworkAccessManagerFactory::setUserAgent(const QString userAgent)
{
    m_userAgent = userAgent;
}
