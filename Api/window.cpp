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
#include "window.h"


Window::Window(QObject *parent) : QObject(parent)
{

}

bool Window::open(const QString url)
{
    emit newTabRequested(QUrl(url));
    return true;
}

bool Window::open(const QString url, const QString windowName)
{
    if(windowName == "_blank") {
        emit newTabRequested(QUrl(url));
    } else {
        emit newWindowRequested(QUrl(url), windowName, "");
    }
    return true;
}

bool Window::open(const QString url, const QString windowName, const QString windowFeatures)
{
    emit newWindowRequested(QUrl(url), windowName, windowFeatures);
    return true;
}
