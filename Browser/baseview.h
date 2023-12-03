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
#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <QUrl>
#include <QSplitter>
#include <QString>
#include <QWidget>

class BaseView
{

public:
    explicit BaseView(QSplitter *splitter) {
        m_splitter = splitter;
    };

    virtual ~BaseView() {};

    static const int TYPE_HTML = 1;

    static const int TYPE_QML = 2;

    virtual void setContent(const QByteArray &content, const QString &mimeType, const QUrl &baseUrl) = 0;

    virtual qreal zoomFactor() = 0;

    virtual void setZoomFactor(const qreal factor) = 0;

    virtual void toggleDevTools() = 0;

    virtual void setUrl(const QUrl &url) = 0;

    virtual const QString title() = 0;

    virtual const QUrl iconUrl() = 0;

    virtual int viewType() = 0;

    virtual void reload() = 0;

protected:

    QSplitter *m_splitter;

    QWidget *m_parent;

};

#endif // BASEVIEW_H
