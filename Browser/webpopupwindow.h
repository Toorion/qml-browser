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
#ifndef WEBPOPUPWINDOW_H
#define WEBPOPUPWINDOW_H

#include <QWidget>
#include "htmlview.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QWebEngineProfile;
class QWebEngineView;
QT_END_NAMESPACE

class WebPopupWindow : public QWidget
{
    Q_OBJECT

public:
    WebPopupWindow(QWebEngineProfile *profile);
    HtmlView *view() const;

private slots:
    void handleGeometryChangeRequested(const QRect &newGeometry);

private:
    QLineEdit *m_urlLineEdit;
    QAction *m_favAction;
    HtmlView *m_view;
};
#endif // WEBPOPUPWINDOW_H
