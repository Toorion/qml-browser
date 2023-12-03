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
#ifndef BROWSERPATHS_H
#define BROWSERPATHS_H

#include <apppaths.h>
#include <QUrl>

class BrowserPaths : public AppPaths
{
public:
    BrowserPaths();

    QLatin1String const static historyManagerName;
    QLatin1String const static downloadManagerName;
    QLatin1String const static settingsManagerName;
    QLatin1String const static blankPageName;
    QLatin1String const static errorPageName;
    QString const static installPageName;

    QString static historyManagerPath();
    QString static downloadManagerPath();
    QString static settingsManagerPath();

    QUrl static installPageUrl();

};

#endif // BROWSERPATHS_H
