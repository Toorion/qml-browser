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
#include "htmlview.h"
#include "mainwindow.h"
#include <QMenu>
#include "webpopupwindow.h"
#include <QSplitter>
#include "browser.h"
#include "tabview.h"


HtmlView::HtmlView(QSplitter *splitter, QWidget *parent, QWebEngineProfile *profile) : QWebEngineView(profile, parent), BaseView(splitter)
{
    QWebEnginePage *webPage = new QWebEnginePage(profile, this);
    setPage(webPage);

    connect(this, &QWebEngineView::titleChanged, dynamic_cast<TabView*>(parent), &TabView::titleChanged);
    connect(this, &QWebEngineView::iconChanged, dynamic_cast<TabView*>(parent), &TabView::iconChanged);
    connect(this, &QWebEngineView::urlChanged, dynamic_cast<TabView*>(parent), &TabView::tabUrlChanged);
    connect(this, &QWebEngineView::loadFinished, dynamic_cast<TabView*>(parent), &TabView::tabLoadFinished);
    connect(this, &QWebEngineView::iconUrlChanged, dynamic_cast<TabView*>(parent), &TabView::tabIconUrlChanged);
    connect(this, &QWebEngineView::loadProgress, dynamic_cast<TabView*>(parent), &TabView::loadProgress);
}

HtmlView::~HtmlView()
{
}


void HtmlView::setContent(const QByteArray &content, const QString &mimeType, const QUrl &baseUrl)
{
    QWebEngineView::setContent(content, mimeType, baseUrl);
}

qreal HtmlView::zoomFactor()
{
    return QWebEngineView::zoomFactor();
}

void HtmlView::setZoomFactor(const qreal factor)
{
    QWebEngineView::setZoomFactor(factor);
}

void HtmlView::toggleDevTools()
{
    if(devView) {
        delete devView;
        devView = nullptr;
    } else {
        if(m_splitter) { // No splitter in popup window
            devView = new QWebEngineView(this);
            m_splitter->addWidget(devView);
            // setStretchFactor doesn't work here
            m_splitter->setSizes(QList<int>({200, 100}));
            devView->page()->setInspectedPage(page());
        }
    }
}

void HtmlView::setUrl(const QUrl &url, const bool m_reload)
{
    if(m_reload) {
        reload();
        return;
    }
    QWebEngineView::setUrl(url);
}

const QString HtmlView::title()
{
    return QWebEngineView::title();
}

const QUrl HtmlView::iconUrl()
{
    return QWebEngineView::iconUrl();
}

void HtmlView::reload()
{
    QWebEngineView::reload();
}

QWebEngineView *HtmlView::createWindow(QWebEnginePage::WebWindowType type)
{
    MainWindow *mainWindow = qobject_cast<MainWindow*>(window());
    if (!mainWindow)
        return nullptr;

    switch (type) {
        case QWebEnginePage::WebBrowserTab: {
            return mainWindow->tabWidget->createActiveTab()->createHtmlView();
        }
        case QWebEnginePage::WebBrowserBackgroundTab: {
            return mainWindow->tabWidget->createTab()->createHtmlView();
        }
        case QWebEnginePage::WebBrowserWindow: {
            TabView *tabView = qobject_cast<TabView*>(mainBrowser.createWindow()->tabWidget->currentWidget());
            return tabView->createHtmlView();
        }
        case QWebEnginePage::WebDialog: {
            WebPopupWindow *popup = new WebPopupWindow(page()->profile());
            connect(popup->view(), &HtmlView::devToolsRequested, this, &HtmlView::devToolsRequest);
            return popup->view();
        }
    }
    return nullptr;
}

void HtmlView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    const QList<QAction *> actions = menu->actions();
    auto inspectElement = std::find(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::InspectElement));
    if (inspectElement == actions.cend()) {
        auto viewSource = std::find(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::ViewSource));
        if (viewSource == actions.cend())
            menu->addSeparator();

        QAction *action = new QAction(menu);
        action->setText("Inspect element");
        connect(action, &QAction::triggered, this, [=]() { devToolsRequest(page()); });

        QAction *before(inspectElement == actions.cend() ? nullptr : *inspectElement);
        menu->insertAction(before, action);
    } else {
        (*inspectElement)->setText(tr("Inspect element"));
    }
    menu->popup(event->globalPos());
}

void HtmlView::devToolsRequest(QWebEnginePage *source)
{
    if(m_splitter) {
        if(!devView) {
            devView = new QWebEngineView(this);
            m_splitter->addWidget(devView);
            // setStretchFactor doesn't work here
            m_splitter->setSizes(QList<int>({200, 100}));
            devView->page()->setInspectedPage(page());
        }
        source->triggerAction(QWebEnginePage::InspectElement);
        emit devToolsRequested(page());
    }
}

