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
#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include "Api_global.h"
#include <QVariantList>
#include <QJSValueIterator>

class API_EXPORT Tools
{
public:
    Tools();

    QString static bytesToText(double bytes);

    QVariantMap static QJSValueToMap(QJSValue obj);
};

#endif // TOOLS_H
