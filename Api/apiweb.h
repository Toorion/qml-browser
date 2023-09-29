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
#ifndef APIWEB_H
#define APIWEB_H

#include <QObject>
#include "apicommon.h"
#include "window.h"
#include "document.h"
#include "locationurl.h"
#include "Api_global.h"

class API_EXPORT ApiWeb : public ApiCommon
{
    Q_OBJECT
    Q_PROPERTY(Document *document READ document CONSTANT)
    Q_PROPERTY(Window *window READ window NOTIFY windowChanged)

public:

    explicit ApiWeb(HttpManager *httpManager, QObject *parent = nullptr);

    Window *window() {return m_window;}

    Document *document() {return m_document;}

    LocationUrl *locationUrl()
    {
        return m_locationUrl;
    }

    void setLocationUrl(const QUrl url)
    {
        m_locationUrl->setUrl(url);
        m_baseUrl = url;
    }

signals:

    void windowChanged();

private:

    Document *m_document;

    Window *m_window;

    LocationUrl *m_locationUrl;

};

#endif // APIWEB_H
