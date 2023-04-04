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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tabwidget.h"
#include "navigationbar.h"
#include <QWebEngineProfile>
#include <QCloseEvent>
#include "../Api/downloadmanagerwidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWebEngineProfile *profile);
    ~MainWindow();

    NavigationBar *navigationBar;

    QSize sizeHint() const override;

    TabWidget *tabWidget;

    void htmlDownloadRequested(QWebEngineDownloadRequest *item);
    void closeDownloadManager();

    DownloadManagerWidget *downloadManagerWidget() {
        return m_downloadManagerWidget;
    };

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QWebEngineProfile *m_profile;

    DownloadManagerWidget *m_downloadManagerWidget = nullptr;

    void handleWebViewTitleChanged(const QString &title);

};
#endif // MAINWINDOW_H
