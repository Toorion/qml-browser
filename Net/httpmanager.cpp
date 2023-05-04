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
#include "httpmanager.h"
#include <QNetworkReply>
#include <QNetworkProxy>
#include "mainapplication.h"
#include <QAuthenticator>
#include "apppaths.h"
#include <QFileInfo>
#include <QDir>
#include "internalnetworkreply.h"
#include <QByteArray>
#include <QFileInfo>

HttpManager::HttpManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
    connect(this, &QNetworkAccessManager::authenticationRequired, this, [this](QNetworkReply *reply, QAuthenticator *auth) {
        authentication(reply->url(), auth);
    });

    connect(this, &QNetworkAccessManager::proxyAuthenticationRequired, this, [this](const QNetworkProxy &proxy, QAuthenticator *auth) {
        proxyAuthentication(proxy.hostName(), auth);
    });
}


void HttpManager::authentication(const QUrl &url, QAuthenticator *auth, QWidget *parent)
{
    QString username;
    QString password;
    if(mainApp->authDialog(username, password,
                           tr("Authorization required"),
                           tr("A username and password are being requested by %1. "
                                "The site says: \"%2\"").arg(url.host(), auth->realm().toHtmlEscaped()), parent)
            ) {
        auth->setUser(username);
        auth->setPassword(password);
    }
}


void HttpManager::proxyAuthentication(const QString &proxyHost, QAuthenticator *auth, QWidget *parent)
{
    const QNetworkProxy proxy = QNetworkProxy::applicationProxy();
    if (!proxy.user().isEmpty() && !proxy.password().isEmpty()) {
        auth->setUser(proxy.user());
        auth->setPassword(proxy.password());
        return;
    }

    QString username;
    QString password;
    if(mainApp->authDialog(username, password, tr("Proxy authorization required"), tr("A username and password are being requested by proxy %1. ").arg(proxyHost), parent)) {
        auth->setUser(username);
        auth->setPassword(password);
    }
}


QFileInfo HttpManager::uniqueFileName(QString path, QUrl url)
{
    QFileInfo info(QDir(path).filePath(url.fileName()));
    QString fileBaseName = info.completeBaseName();
    if(info.exists()) {
        QString fileName;
        for(int i = 1; i < 99999; i++) {
            fileName = fileBaseName + " (" + QString::number(i) + ")." + info.suffix();
            info.setFile(info.path() + QDir::separator() + fileName);
            if(!info.exists()) {
                return info;
            }
        }
    }
    return info;
}



DownloadItem *HttpManager::download(QUrl url)
{
    QFileInfo fileInfo = uniqueFileName(
                AppPaths::downloadPath(),
                url);

    auto item = new DownloadItem(url);

    item->setDownloadDirectory(fileInfo.absolutePath());
    item->setDownloadFileName(fileInfo.fileName());

    if(!item->openFile()) {
        return nullptr;
    }

    item->setReply(get(QNetworkRequest(url)));

    return item;
}


void HttpManager::setUserAgent(const QString userAgent)
{
    this->m_userAgent = userAgent;
}


QNetworkReply *HttpManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    if(request.url().scheme() == INTERNAL_URL_SCHEME) {

        InternalNetworkReply *reply = new InternalNetworkReply();

        QFile toolAppFile(AppPaths::toolPath(request.url()));
        if(toolAppFile.exists() && toolAppFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            reply->setHttpStatusCode( 200 );
            reply->setContentType(QByteArray::fromStdString(QML_MIME.data()));
            reply->setUrl(request.url());
            reply->setContent(toolAppFile.readAll());

            toolAppFile.close();
            toolAppFile.deleteLater();
        } else {
            reply->setHttpStatusCode( 404, "Not Found" );
            reply->setContentType("text/html");
            reply->setContent( QString("<html><body><h1>Page Not Found</h1></body></html>") );
        }

        return reply;
    }

    QNetworkRequest req = request;
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());

    if(m_reloading)
        req.setAttribute(QNetworkRequest::Attribute::CacheLoadControlAttribute, QNetworkRequest::CacheLoadControl::AlwaysNetwork);

    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}

