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
#include <QWindow>
#include <QDebug>

Window::Window(QObject *parent) : QObject(parent)
{

}

bool Window::open(const QString url)
{
    emit newTabRequested(url);
    return true;
}

bool Window::open(const QString url, const QString windowName)
{
    if(windowName == "_blank") {
        emit newTabRequested(url);
    } else {
        emit newWindowRequested(url, windowName, "");
    }
    return true;
}

bool Window::open(const QString url, const QString windowName, const QString windowFeatures)
{
    emit newWindowRequested(url, windowName, windowFeatures);
    return true;
}

void Window::setWidth(int width)
{
    if(m_width != width) {
        m_width = width;
        emit widthChanged();
    }
}

void Window::setHeight(int height)
{
    if(m_height != height) {
        m_height = height;
        emit heightChanged();
    }
}

