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
#ifndef APPPATHS_H
#define APPPATHS_H

#include <QString>
#include "App_global.h"
#include <QUrl>

class APP_EXPORT AppPaths
{
public:
    explicit AppPaths();
    ~AppPaths();

    QString static rootPath();

    QString static iconsPath();

    QString static toolsPath();

    QString static toolPath(QUrl const requestUrl);

    QString static currentProfilePath();

    QString static cachePath();

    QString static dataPath();

    QString static downloadPath();

    QString const iconsDir = QLatin1String("/icons");

    QString const toolsDir = QLatin1String("/tools");

    QString const qmlMainFile = QLatin1String("main.qml");

    QString const htmlMainFile = QLatin1String("index.html");

private:
    void init();

    QString m_appPath;

    QString m_dataPath;

    QString m_currentProfilePath;

    QString m_cachePath;

    QString m_downloadPath;
};

#endif // APPPATHS_H
