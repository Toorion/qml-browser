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
#include "webpopupwindow.h"
#include <QAction>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWindow>
#include "htmlview.h"
#include <QWebEnginePage>


WebPopupWindow::WebPopupWindow(QWebEngineProfile *profile)
    : m_urlLineEdit(new QLineEdit(this))
    , m_favAction(new QAction(this))
    , m_view(new HtmlView(nullptr, this, profile))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    layout->addWidget(m_urlLineEdit);
    layout->addWidget(m_view);

    m_view->setPage(new QWebEnginePage(profile, m_view));
    m_view->setFocus();

    m_urlLineEdit->setReadOnly(true);
    m_urlLineEdit->addAction(m_favAction, QLineEdit::LeadingPosition);

    connect(m_view, &QWebEngineView::titleChanged, this, &QWidget::setWindowTitle);
    connect(m_view, &QWebEngineView::urlChanged, [this](const QUrl &url) {
        m_urlLineEdit->setText(url.toDisplayString());
    });
    connect(m_view, &HtmlView::iconChanged, m_favAction, &QAction::setIcon);
    connect(m_view->page(), &QWebEnginePage::geometryChangeRequested, this, &WebPopupWindow::handleGeometryChangeRequested);
    connect(m_view->page(), &QWebEnginePage::windowCloseRequested, this, &QWidget::close);
}

HtmlView *WebPopupWindow::view() const
{
    return m_view;
}

void WebPopupWindow::handleGeometryChangeRequested(const QRect &newGeometry)
{
    if (QWindow *window = windowHandle())
        setGeometry(newGeometry.marginsRemoved(window->frameMargins()));
    show();
    m_view->setFocus();
}
