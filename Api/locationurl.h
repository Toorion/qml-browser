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
#ifndef LOCATIONURL_H
#define LOCATIONURL_H

#include <QObject>
#include <QUrl>
#include "Api_global.h"

class API_EXPORT LocationUrl : public QObject
{
    Q_OBJECT
public:
    explicit LocationUrl(QObject *parent = nullptr);

    QUrl url() {return m_url;}

    void setUrl(const QUrl url) {
        m_url = url;
    }

    bool setRawUrl(const QString rawUrl);

signals:

    void urlChanged(const QUrl &url);

private:

    QUrl m_url;

};

#endif // LOCATIONURL_H
