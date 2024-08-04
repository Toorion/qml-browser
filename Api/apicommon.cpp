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
#include "apicommon.h"
#include <QMessageBox>
#include "qi.h"
#include "downloaditem.h"
#include <QStandardPaths>
#include <QEventLoop>
#include "networkdiskcache.h"
#include "urlhelper.h"

ApiCommon::ApiCommon(NetworkAccessManagerFactory *networkManagerFactory, QObject *parent)
    : QObject(parent),
      m_networkManagerFactory(networkManagerFactory)
{
    m_qi = new Qi(this);
    m_log = new Log(this);
}

void ApiCommon::alert(const QString message, const QString title)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.exec();
}

bool ApiCommon::confirm(const QString message, const QString title)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    return msgBox.exec() == QMessageBox::AcceptRole;
}

DownloadItem *ApiCommon::download(const QString url)
{
    DownloadItem *item = httpManager()->download(QUrl(url));
    m_qi->downloadManagerWidget()->downloadRequested(item);
    m_qi->downloadManagerWidget()->setVisible(true);
    return item;
}

QString ApiCommon::preload(const QString inputUrl)
{
    QUrl url(inputUrl);
    if(url.isRelative()) {
        url = m_baseUrl.resolved(url);
    } else {
        url = QUrl::fromUserInput(inputUrl);
    }

    QUrl u = UrlHelper::urlToLocalPath(url);
    if(u.isLocalFile()) {
        return "file:/" + u.toLocalFile();
    }

    QNetworkReply *reply = httpManager()->get(QNetworkRequest(url));

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, [&loop]() {
        // by completion of the request
        loop.quit();
    });

    loop.exec();

    if(reply->error() != QNetworkReply::NoError) {
        m_log->error(QLatin1String("File %1 load error").arg(url.toString()));
    }

    reply->close();
    reply->deleteLater();

    return "file:/" + (static_cast<NetworkDiskCache*>(httpManager()->cache()))->filePath(url);
}

void ApiCommon::setAccessRights(AccessRights *accessRights)
{
    m_accessRights = accessRights;
}

AccessRights *ApiCommon::accessRights()
{
    return m_accessRights;
}

HttpManager *ApiCommon::httpManager()
{
    if(nullptr == m_httpManager) {
        m_httpManager = qobject_cast<HttpManager*>(m_networkManagerFactory->create(this));
    }
    return m_httpManager;
}

