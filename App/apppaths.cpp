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

Q_GLOBAL_STATIC(AppPaths, gs_app_paths)

AppPaths::AppPaths()
{
    init();
}

AppPaths::~AppPaths()
{
}

QString AppPaths::rootPath()
{
    AppPaths* p = gs_app_paths();
#ifdef Q_OS_WIN
    return p->m_appPath;
#else
    return p->m_appPath + QLatin1String("/..");
#endif
}

QString AppPaths::iconsPath()
{
    AppPaths* p = gs_app_paths();
    return rootPath() + p->iconsDir;
}

QString AppPaths::toolsPath()
{
    AppPaths* p = gs_app_paths();
    return rootPath() + p->toolsDir;
}

QString AppPaths::toolPath(const QUrl requestUrl)
{
    AppPaths* p = gs_app_paths();

    QString uri = requestUrl.adjusted(QUrl::RemoveScheme | QUrl::StripTrailingSlash).toString().remove(0,2);

    if(uri.count('/') == 0) {
            uri += QLatin1Char('/') + p->qmlMainFile;
        }

    return QUrl::fromLocalFile(
        toolsPath() + QLatin1Char('/') + QDir::cleanPath(uri)
    ).toLocalFile();
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
    return p->m_dataPath;
}

QString AppPaths::downloadPath()
{
    AppPaths* p = gs_app_paths();
    return p->m_downloadPath;
}

void AppPaths::init()
{
    m_appPath = QCoreApplication::applicationDirPath();
    m_dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_currentProfilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    m_cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    m_downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (m_dataPath.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir dataDir(m_dataPath);
    if(!dataDir.mkpath(dataDir.absolutePath())) {
        qFatal("wrong dataPath");
    }
}
