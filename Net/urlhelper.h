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
#ifndef URLHELPER_H
#define URLHELPER_H

#include <QUrl>
#include "Net_global.h"

#define INTERNAL_URL_SCHEME QLatin1String("internal")
#define DAPP_URL_SCHEME QLatin1String("dapp")
#define QML_MAIN_FILE QString("main.qml")
#define HTML_MAIN_FILE QString("main.html")
#define DIR_ALTER_SP QLatin1Char('@')

class NET_EXPORT UrlHelper
{
public:
    UrlHelper();

    bool static isLocalSource(const QUrl &url);

    QUrl static urlToLocalPath(const QUrl &url, bool toDir = false);

    QUrl static assertFilename(const QUrl &url);

    QString static urlLocalPath(const QUrl &url);

    QUrl static gitToDappUrl(const QUrl &url);


};

#endif // URLHELPER_H
