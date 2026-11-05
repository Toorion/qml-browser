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
#include <QJSValue>
#include <QUrl>
#include <QVariantMap>
#include <QJSEngine>
#include "apicommon.h"
#include "window.h"
#include "document.h"
#include "locationurl.h"
#include "fetchapi.h"
#include "Api_global.h"
#include "qmlvalue.h"

class API_EXPORT ApiWeb : public ApiCommon
{
    Q_OBJECT
    Q_PROPERTY(Document *document READ document CONSTANT)
    Q_PROPERTY(Window *window READ window CONSTANT)
    Q_PROPERTY(QmlValue *navInput READ navInput CONSTANT)
    Q_PROPERTY(FetchApi *fetchApi READ fetchApi CONSTANT)

public:

    explicit ApiWeb(NetworkAccessManagerFactory *networkManagerFactory, QJSEngine *engine, QObject *parent = nullptr);

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

    QmlValue *navInput()
    {
        return m_navInput;
    }

    void setNavInput(const QString &text) {
        m_navInput->setValue(new QVariant(text));
    }

    FetchApi *fetchApi() { return m_fetchApi; }

    // Convenience method to call fetch directly from ApiWeb
    Q_INVOKABLE QJSValue fetch(const QUrl &url, const QVariantMap &options = QVariantMap());

signals:

private:

    Document *m_document;

    Window *m_window;

    LocationUrl *m_locationUrl;

    QmlValue *m_navInput;

    FetchApi *m_fetchApi = nullptr;

};
#endif // APIWEB_H
