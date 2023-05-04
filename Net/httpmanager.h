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
#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QNetworkAccessManager>
#include "downloaditem.h"
#include <QFileInfo>
#include "Net_global.h"

#define INTERNAL_URL_SCHEME QLatin1String("internal")
#define QML_MIME QLatin1String("text/qml")
#define QML_EXT QLatin1String("qml")
#define HTML_MIME QLatin1String("text/html")
#define HTML_EXT QLatin1String("html")

class NET_EXPORT HttpManager : public QNetworkAccessManager
{
    Q_OBJECT
public:

    explicit HttpManager(QObject *parent = Q_NULLPTR);

    void authentication(const QUrl &url, QAuthenticator *auth, QWidget *parent = Q_NULLPTR);
    void proxyAuthentication(const QString &proxyHost, QAuthenticator *auth, QWidget *parent = Q_NULLPTR);

    QFileInfo uniqueFileName(QString path, QUrl url);
    DownloadItem *download(QUrl url);
    void setUserAgent(const QString userAgent);

    void setReloading(const bool reloading) {
        m_reloading = reloading;
    };

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) override;
private:

    QString m_userAgent;

    QList<DownloadItem *> m_downloadItems;

    bool m_reloading = false;
};

#endif // HTTPMANAGER_H
