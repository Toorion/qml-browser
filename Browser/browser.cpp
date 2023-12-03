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
#include "browser.h"
#include "mainwindow.h"
#include <QWebEngineProfile>
#include <QApplication>
#include "historydb.h"
#include "historyitemmodel.h"
#include "browsersettings.h"


Browser::Browser()
{
    HistoryItemModel *historyItemModel = &HistoryItemModel::instance();
    historyItemModel->fill(HistoryDb::list());

    QObject::connect(QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested,
                     this, [=] (QWebEngineDownloadRequest *item) {
        MainWindow *window = qobject_cast<MainWindow*>(QApplication::activeWindow());
        if(window) {
            window->htmlDownloadRequested(item);
        }
    });
}


Browser &Browser::instance()
{
    static Browser instance;
    return instance;
}


MainWindow *Browser::createWindow(bool offTheRecord)
{
    if (!m_profile) {
        m_profile.reset(new QWebEngineProfile(
                QString::fromLatin1("qbqbrowser.%1").arg(qWebEngineChromiumVersion())));

        QObject::connect(m_profile.get(), &QWebEngineProfile::downloadRequested, this, [] (QWebEngineDownloadRequest *item) {
            MainWindow *window = qobject_cast<MainWindow*>(QApplication::activeWindow());
            if(window) {
                window->htmlDownloadRequested(item);
            }
        });
    }
    auto profile = offTheRecord ? QWebEngineProfile::defaultProfile() : m_profile.get();
    profile->setHttpUserAgent(browserSettings->appUserAgent());

    auto mainWindow = new MainWindow(profile);

    connect(mainWindow->navigationBar->newWindowAction, &QAction::triggered, this, &Browser::createWindow);
    connect(mainWindow->navigationBar->newIncognitoAction, &QAction::triggered, this, [this]() {
        this->createWindow(true);
    });

    mainWindows.append(mainWindow);
    QObject::connect(mainWindow, &QObject::destroyed, this, [this, mainWindow]() {
        mainWindows.removeOne(mainWindow);
    });
    mainWindow->show();
    return mainWindow;
}

