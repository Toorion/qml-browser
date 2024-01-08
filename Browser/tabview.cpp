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
#include "tabview.h"
#include <QVBoxLayout>
#include "qmlview.h"
#include <QToolBar>
#include "htmlview.h"
#include <QSplitter>
#include <QLabel>
#include <QString>
#include <QWebEngineView>
#include <QUrl>
#include <QMessageBox>
#include "historyitemmodel.h"
#include "historyitem.h"
#include <QWebEnginePage>
#include "searchengine.h"
#include "browserpaths.h"
#include "urlhelper.h"



TabView::TabView(QWebEngineProfile *profile, QWidget *parent) : QWidget(parent),
  m_profile(profile)
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_splitter = new QSplitter(Qt::Vertical, this);
    layout->addWidget(m_splitter);

    setLayout(layout);
}

TabView::~TabView()
{
    if(m_pageView) {
        delete m_pageView;
    }
}

void TabView::loadUrl(const QString &rawUrl)
{
    QUrl url = QUrl::fromUserInput(rawUrl);
    if(!url.isValid()) {
        url = SearchEngine::searchUrl(rawUrl);
    } else if(url.fileName().endsWith(QLatin1String(".git"), Qt::CaseInsensitive)){
        return installUrl(rawUrl);
    }
    setUrl(url);
}

void TabView::installUrl(const QString &rawUrl)
{
    m_installationUrl = new QUrl(QUrl::fromUserInput(rawUrl));
    if(m_installationUrl->isEmpty()) {
        // todo: Goto error page
        qWarning("Invalid URL");
        return;
    }
    if(DappInstaller::instance().proecessIsActive()) {
        // todo: Goto error page
        qWarning("Installation already in progress");
        return;
    }
    setUrl(BrowserPaths::installPageUrl());
}

void TabView::reload()
{
    if(pageView()) {
        m_pageView->reload();
    }
}

void TabView::setUrl(const QUrl &url)
{
    QNetworkRequest req(url);
    NetworkAccessManagerFactory networkManagerFactory;
    auto httpManager = networkManagerFactory.create(this);
    if(UrlHelper::isLocalSource(url)) {
        m_reply = httpManager->get(req);
    } else {
        if(m_reload) {
            req.setAttribute(QNetworkRequest::Attribute::CacheLoadControlAttribute, QNetworkRequest::CacheLoadControl::AlwaysNetwork);
            m_reload = false;
        }
        m_reply = httpManager->head(req);
    }
    connect(m_reply, &QNetworkReply::finished, this, &TabView::loadFinished);
}

void TabView::loadFinished()
{
    if(m_pageView) {
        delete m_pageView;
        m_pageView = nullptr;
        while (m_splitter->count()>0) {
          delete m_splitter->widget(0);
        }
    }

    QString mimeType = m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
    if(mimeType.compare(QML_MIME, Qt::CaseInsensitive) == 0) {
        m_pageView = new QmlView (m_splitter, this);
    } else {
        m_pageView = new HtmlView(m_splitter, this, m_profile);
    }
    if(UrlHelper::isLocalSource(m_reply->url())) {
        m_pageView->setContent(m_reply->readAll(), mimeType, m_reply->url());
        if(m_installationUrl) {
          static_cast<QmlView*>(m_pageView)->setInstallationUrl(m_installationUrl);
        }
    } else {
        m_pageView->setUrl(m_reply->url());
    }

    m_splitter->insertWidget(0, dynamic_cast<QWidget*>(m_pageView));
    tabUrlChanged(m_reply->url());

    m_reply->deleteLater();
}


void TabView::tabUrlChanged(const QUrl url)
{
    m_currentUrl = url;
    emit urlChanged(m_currentUrl);
}

void TabView::tabLoadFinished(bool ok)
{
    if(ok) {
        if(m_historyItem) {
            historyBack.push(m_historyItem);
        }
        m_historyItem = new HistoryItem(m_currentUrl, m_pageView->viewType(), m_pageView->title(), m_pageView->iconUrl());
        HistoryItemModel::instance().addHistoryItem(m_historyItem);
    }
}

void TabView::tabIconUrlChanged(const QUrl iconUrl)
{
    if(m_historyItem) {
        m_historyItem->setIconUrl(iconUrl);
        HistoryItemModel::instance().updateHistoryItem(m_historyItem);
    }
}



QWebEngineView *TabView::createHtmlView()
{
    m_pageView = new HtmlView(m_splitter, this, m_profile);
    return dynamic_cast<QWebEngineView *> (m_pageView);
}


void TabView::back()
{
    HistoryItem *historyItem = historyBack.pop();
    if(!m_currentUrl.isEmpty()) {
        historyForward.push(new HistoryItem(m_currentUrl, m_pageView->viewType(), m_pageView->title(), m_pageView->iconUrl()));
    }
    setUrl(historyItem->url());
}

void TabView::forward()
{
    HistoryItem *historyItem = historyForward.pop();
    setUrl(historyItem->url());
}
