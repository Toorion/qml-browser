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
#ifndef TABVIEW_H
#define TABVIEW_H

#include <QWidget>
#include <QNetworkReply>
#include <baseview.h>
#include <QUrl>
#include <QStack>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QSplitter>
#include <QIcon>
#include <QWebEngineProfile>
#include "historyitem.h"
#include "networkaccessmanagerfactory.h"

class TabView : public QWidget
{
    Q_OBJECT
public:
    explicit TabView(QWebEngineProfile *profile, QWidget *parent = nullptr);
    ~TabView();

    BaseView *pageView() {
        return m_pageView;
    };

    void loadUrl(const QString &rawUrl);

    void installUrl(const QString &rawUrl);

    void reload();

    void setUrl(const QUrl &url);

    void loadFinished();



    QUrl getCurrentUrl() {
        return m_currentUrl;
    };

    QWebEngineView *createHtmlView();

    QStack<HistoryItem*> historyBack;

    QStack<HistoryItem*> historyForward;


public Q_SLOTS:
    void back();
    void forward();
    void tabUrlChanged(const QUrl url);
    void tabLoadFinished(bool ok);
    void tabIconUrlChanged(const QUrl iconUrl);

signals:
    void titleChanged(const QString &title);
    void iconChanged(const QIcon &icon);
    void urlChanged(const QUrl url);
    void loadProgress(int progress);


private:

    BaseView *m_pageView = nullptr;

    QUrl m_currentUrl;

    QNetworkReply *m_reply;

    HistoryItem *m_historyItem = nullptr;

    QWebEngineProfile *m_profile;

    QSplitter *m_splitter;

    bool m_reload = false;

    QUrl *m_installationUrl = nullptr;

};

#endif // TABVIEW_H
