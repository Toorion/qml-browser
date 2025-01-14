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
#include "apppaths.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QUrl>
#include <QByteArray>

Q_GLOBAL_STATIC(AppPaths, gs_app_paths)

AppPaths::AppPaths()
{
}

AppPaths::~AppPaths()
{
}

void AppPaths::init(const QString &dataPath)
{
    AppPaths* p = gs_app_paths();

    QString appPath = QCoreApplication::applicationDirPath();
    if (!appPath.endsWith(QLatin1Char('/')))
        appPath.append(QLatin1Char('/'));
    p->m_appPath = QUrl::fromLocalFile(appPath);

#ifndef Q_OS_WIN
    p->m_appPath = p->m_appPath.resolved(QUrl(".."));
#endif

    p->m_currentProfilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    p->m_downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (dataPath == nullptr) {
        p->m_dataPath = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        if (p->m_dataPath.isEmpty()) qFatal("Cannot determine settings storage location");
    } else {
        p->m_dataPath = dataPath;
    }

    QDir dataDir(p->m_dataPath.toLocalFile());
    if(!dataDir.mkpath(dataDir.absolutePath())) {
        qFatal("wrong dataPath");
    }

    p->m_cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    if (!p->m_cachePath.endsWith(QLatin1Char('/')))
        p->m_cachePath += QLatin1Char('/');
}

QString AppPaths::rootPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_appPath.toLocalFile();
}

QUrl AppPaths::resolved(const QString path)
{
    AppPaths* p = gs_app_paths();
    return p->m_appPath.resolved(path);
}

QUrl AppPaths::iconsPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_appPath.resolved(p->iconsDir);
}

QUrl AppPaths::toolsPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_appPath.resolved(p->toolsDir);
}

QUrl AppPaths::toolPath(QUrl url)
{
    QString uri = url.adjusted(QUrl::RemoveScheme | QUrl::StripTrailingSlash).toString().remove(0,2);
    return toolsPath().resolved(uri);
}

QString AppPaths::currentProfilePath()
{
    AppPaths* p = gs_app_paths();
    return p->m_currentProfilePath;
}

QString AppPaths::cachePath()
{
    AppPaths* p = gs_app_paths();
    return p->m_cachePath;
}

QString AppPaths::dataPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_dataPath.toLocalFile();
}

QString AppPaths::dbPath()
{
    return dataPath();
}

QUrl AppPaths::dappsPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_dataPath.resolved(p->dappsDir);
}

QString AppPaths::downloadPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_downloadPath;
}

QString AppPaths::webAppPath(const QUrl &url)
{
    AppPaths* p = gs_app_paths();

    QUrl cleanUrl;
    cleanUrl.setScheme(url.scheme());
    cleanUrl.setHost(url.host());

    quint16 id = qChecksum(cleanUrl.toEncoded());
    QString strId = QString::number(id, 16);

    return p->m_cachePath + strId.left(2) + DIR_SP + strId.right(2) + DIR_SP;
}

QUrl AppPaths::dappPath(const QUrl &internalUrl)
{
    return dappsPath().resolved(internalUrl);
}

