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
#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QUrl>
#include "locationurl.h"
#include "Api_global.h"

class API_EXPORT Location
{
    Q_GADGET
    Q_PROPERTY(QString href READ href WRITE setHref)

public:
    explicit Location();

    void setLocationUrl(LocationUrl * locationUrl)
    {
        m_locationUrl = locationUrl;
    };

    QString href()
    {
        return m_locationUrl->url().toString();
    };

    bool setHref(QString href)
    {
        return m_locationUrl->setRawUrl(href);
    }

    QString toString() const
    {
        return m_locationUrl->url().toString();
    }

    friend inline bool operator!=(const Location& lhs, const Location& rhs)
    {

        return !(lhs.m_locationUrl == rhs.m_locationUrl);
    }

    friend inline bool operator==(const Location& lhs, const Location& rhs)
    {
        return (lhs.m_locationUrl == rhs.m_locationUrl); // Reqursive call
    }

signals:

private:
    LocationUrl *m_locationUrl;
};

Q_DECLARE_METATYPE(Location)


#endif // LOCATION_H
