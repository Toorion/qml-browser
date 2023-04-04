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
#ifndef QMLVIEW_H
#define QMLVIEW_H

#include <QWidget>
#include <QQmlComponent>
#include <QUrl>
#include <baseview.h>
#include <QQuickView>
#include <QLayout>
#include <QQuickItem>
#include <QSplitter>
#include "qmldevtools.h"
#include <QQmlEngine>
#include "apiweb.h"
#include <QString>
#include <QThread>
#include <QNetworkReply>

class QmlView : public QWidget, public BaseView
{
    Q_OBJECT
public:
    explicit QmlView(QSplitter *splitter, QWidget *parent);

    ~QmlView();

    static BaseView* create(const QByteArray &html, const QUrl &baseUrl, QLayout *layout);

    void setContent(const QByteArray &content, const QString &mimeType, const QUrl &baseUrl) override;

    qreal zoomFactor() override;

    void setZoomFactor(const qreal factor) override;

    void toggleDevTools() override;

    int viewType() override {return BaseView::TYPE_QML;};

    void setUrl(const QUrl &url, const bool reload = false) override;

    const QString title() override;

    const QUrl iconUrl() override;

public Q_SLOTS:
    void reload() override;
    void continueLoad();
    void indexLoaded();

signals:

    void titleChanged(const QString &title);
    void iconChanged(const QIcon &icon);
    void urlChanged(const QUrl &url);
    void loadFinished(const bool &ok);


private:

    QQmlEngine m_qmlEngine;

    QWidget *m_container;

    QQmlComponent *m_component;

    QUrl m_url;

    QQuickView *m_quickView;

    QQmlContext *m_context;

    QQuickItem *m_contentItem = nullptr;

    QmlDevTools *m_devTools = nullptr;

    ApiWeb *m_api;

    QString m_title;

    QUrl m_iconUrl;

    void deepClean(const QObjectList list);

    QNetworkReply *m_reply;

};

#endif // QMLVIEW_H
