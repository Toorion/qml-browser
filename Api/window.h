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
#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <QUrl>
#include "location.h"
#include "Api_global.h"

class API_EXPORT Window : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Location location MEMBER location)
    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)

public:
    explicit Window(QObject *parent = nullptr);

    Q_INVOKABLE bool open(const QString url);
    Q_INVOKABLE bool open(const QString url, const QString windowName);
    Q_INVOKABLE bool open(const QString url, const QString windowName, const QString windowFeatures);

    Location location;

    void setWidth(int width);
    void setHeight(int width);

    int width() {
        return m_width;
    }

    int height() {
        return m_height;
    }


signals:

    void newTabRequested(const QString url);
    void newWindowRequested(const QString url, const QString windowName, const QString windowFeatures);
    void widthChanged();
    void heightChanged();

private:

    int m_width;
    int m_height;

};

#endif // WINDOW_H
