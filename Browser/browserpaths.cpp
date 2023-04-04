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
#include "browserpaths.h"
#include <QGlobalStatic>

Q_GLOBAL_STATIC(BrowserPaths, gs_browser_paths)

QLatin1String const BrowserPaths::downloadManagerName = QLatin1String("downloads");

QLatin1String const BrowserPaths::historyManagerName = QLatin1String("history");

QLatin1String const BrowserPaths::settingsManagerName = QLatin1String("settings");

BrowserPaths::BrowserPaths()
{

}

QString BrowserPaths::historyManagerPath()
{
    BrowserPaths* p = gs_browser_paths();
    return p->toolsPath() + QLatin1Char('/') + historyManagerName + QLatin1Char('/') + p->qmlMainFile;
}

QString BrowserPaths::downloadManagerPath()
{
    BrowserPaths* p = gs_browser_paths();
    return p->toolsPath() + QLatin1Char('/') + downloadManagerName + QLatin1Char('/') + p->qmlMainFile;
}

QString BrowserPaths::settingsManagerPath()
{
    BrowserPaths* p = gs_browser_paths();
    return p->toolsPath() + QLatin1Char('/') + settingsManagerName + QLatin1Char('/') + p->qmlMainFile;
}

