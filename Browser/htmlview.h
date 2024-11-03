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
#ifndef HTMLVIEW_H
#define HTMLVIEW_H

#include <QWidget>
#include <baseview.h>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QSplitter>
#include <QWebEngineProfile>

class HtmlView : public QWebEngineView, public BaseView
{
    Q_OBJECT
public:
    explicit HtmlView(QSplitter *splitter, QWidget *parent, QWebEngineProfile *profile);

    ~HtmlView();

    void setContent(const QByteArray &content, const QString &mimeType, const QUrl &baseUrl) override;

    qreal zoomFactor() override;

    void setZoomFactor(const qreal factor) override;

    void toggleDevTools() override;

    void setUrl(const QUrl &url) override;

    int viewType() override {return BaseView::TYPE_HTML;};

    const QString title() override;

    const QUrl iconUrl() override;

    const QString description() override;

    void navTyped(const QString &text) override {
        Q_UNUSED(text);
    };

    bool addToBookmark() override;

public Q_SLOTS:
    void reload() override;

signals:
    void clickEmitted(const QUrl url);
    void devToolsRequested(QWebEnginePage *source);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void devToolsRequest(QWebEnginePage *source);

    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

private:
    QSplitter *layout;

    QWebEngineView *devView = nullptr;

    QString m_description;

};

#endif // HTMLVIEW_H
