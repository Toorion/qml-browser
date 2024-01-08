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
#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include "urllineedit.h"

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <tabview.h>
#include <QUrl>
#include <QShortcut>


class NavigationBar : public QToolBar
{
    Q_OBJECT
public:
    explicit NavigationBar(QWidget *parent = nullptr);

    void connectTab(TabView *tabView);

    QAction *historyBackAction;
    QAction *historyForwardAction;
    QAction *stopReloadAction;

    UrlLineEdit *urlLineEdit;

    QAction *favAction;

    QAction *newTabAction;
    QAction *newWindowAction;
    QAction *newIncognitoAction;

    QAction *historyAction;
    QAction *downloadsAction;

    QAction *settingsAction;

    TabView *activeTabView = nullptr;

public slots:
    void changeUrl(const QUrl url);
    void handleLoadProgress(int progress);

signals:
    void loadProgress(int progress);

private:
    QShortcut *m_closeTabShortcut;

};

#endif // NAVIGATIONBAR_H
