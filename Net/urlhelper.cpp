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
#include <QFile>
#include "urlhelper.h"
#include "apppaths.h"
#include <QFileInfo>


UrlHelper::UrlHelper()
{

}

bool UrlHelper::isLocalSource(const QUrl &url)
{
    return url.isLocalFile() ||
           (url.scheme().compare(INTERNAL_URL_SCHEME) == 0) ||
           (url.scheme().compare(DAPP_URL_SCHEME) == 0);
}

QUrl UrlHelper::urlToLocalPath(const QUrl &url, bool toDir)
{
    if(url.isLocalFile()) {
        return url;
    } else if(url.scheme().compare(INTERNAL_URL_SCHEME) == 0) {
        return assertFilename(AppPaths::toolPath(url));
    } else if (url.scheme().compare(DAPP_URL_SCHEME) == 0) {
        QString uri = url.adjusted(QUrl::RemoveScheme | QUrl::StripTrailingSlash).toString().remove(0,2);
        QUrl u = AppPaths::dappsPath().resolved(uri);
        if(toDir) {
            return u;
        }
        return assertFilename(u);
    }
    return QUrl();

}

QUrl UrlHelper::assertFilename(const QUrl &url)
{
    if(!url.fileName().isEmpty()) {
        QFileInfo fi(url.toLocalFile());
        if(fi.isFile() && fi.exists()) {
            return url;
        }
    }

    QUrl u(url);
    u.setPath(url.path() + DIR_SP);

    QUrl qu = u.resolved(QML_MAIN_FILE);
    QFile qf(qu.toLocalFile());
    if(qf.exists()) {
        return qu;
    }
    QUrl hu = u.resolved(HTML_MAIN_FILE);
    QFile hf(hu.toLocalFile());
    if(hf.exists()) {
        return hu;
    }

    return url;
}

QString UrlHelper::urlLocalPath(const QUrl &url)
{
    if(url.scheme().compare(INTERNAL_URL_SCHEME) == 0) {
        return AppPaths::dataPath();
    } else if (url.scheme().compare(DAPP_URL_SCHEME) == 0) {
        return AppPaths::dappPath(url).toLocalFile();
    }
    return AppPaths::webAppPath(url);
}

QString UrlHelper::urlLocalDataPath(const QUrl &url)
{
    if(url.scheme().compare(INTERNAL_URL_SCHEME) == 0) {
        return AppPaths::toolPath(url).toLocalFile();
    } else if (url.scheme().compare(DAPP_URL_SCHEME) == 0) {
        return AppPaths::dappPath(url).toLocalFile();
    }
    return AppPaths::webAppPath(url);
}

QUrl UrlHelper::gitToDappUrl(const QUrl &url)
{
    QUrl u;
    u.setScheme(DAPP_URL_SCHEME);
    QStringList parts = url.path().split(DIR_SP, Qt::SkipEmptyParts);
    QString host = url.host();
    if(parts.size() > 0) {
        for(qsizetype i = 0; i < parts.size(); i++) {
            if(i == parts.size() - 1) {
                host = parts[i].section(".",0,0) + '.' + host;
            } else {
                host = parts[i] + '.' + host;
            }
        }
    }
    u.setHost(host);
    return u;

}




















