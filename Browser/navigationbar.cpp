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
#include "navigationbar.h"
#include <QAction>
#include <QList>
#include <QKeySequence>
#include <QIcon>
#include <QLineEdit>
#include <QToolButton>
#include <QMenu>
#include <QShortcut>
#include <QCoreApplication>
#include <tabview.h>
#include <QFile>
#include <QApplication>
#include "tabwidget.h"
#include "browserpaths.h"
#include "httpmanager.h"
#include "aboutdialog.h"

NavigationBar::NavigationBar(QWidget *parent) : QToolBar(parent)
{
    setMovable(false);
    toggleViewAction()->setEnabled(false);

    /**
     * History Back button
     */
    historyBackAction = new QAction(this);
    historyBackAction->setEnabled(false);
    QList<QKeySequence> backShortcuts = QKeySequence::keyBindings(QKeySequence::Back);
    for (auto it = backShortcuts.begin(); it != backShortcuts.end();) {
        // Chromium already handles navigate on backspace when appropriate.
        if ((*it)[0].key() == Qt::Key_Backspace)
            it = backShortcuts.erase(it);
        else
            ++it;
    }
    // For some reason Qt doesn't bind the dedicated Back key to Back.
    backShortcuts.append(QKeySequence(Qt::Key_Back));
    historyBackAction->setShortcuts(backShortcuts);
    historyBackAction->setIconVisibleInMenu(false);
    QIcon backIcon;
    backIcon.addPixmap(QPixmap("icons:left.png"), QIcon::Normal);
    backIcon.addPixmap(QPixmap("icons:left_disabled.png"), QIcon::Disabled);
    historyBackAction->setIcon(backIcon);
    historyBackAction->setToolTip(tr("Go back in history"));
    addAction(historyBackAction);
    connect(historyBackAction, &QAction::triggered, [this]() {
        if (activeTabView)
            activeTabView->back();
    });



    /**
     * History Forward button
     */
    historyForwardAction = new QAction(this);
    historyForwardAction->setEnabled(false);
    QList<QKeySequence> fwdShortcuts = QKeySequence::keyBindings(QKeySequence::Forward);
    for (auto it = fwdShortcuts.begin(); it != fwdShortcuts.end();) {
        if (((*it)[0].key() & Qt::Key_unknown) == Qt::Key_Backspace)
            it = fwdShortcuts.erase(it);
        else
            ++it;
    }
    fwdShortcuts.append(QKeySequence(Qt::Key_Forward));
    historyForwardAction->setShortcuts(fwdShortcuts);
    historyForwardAction->setIconVisibleInMenu(false);
    QIcon forwardIcon;
    forwardIcon.addPixmap(QPixmap("icons:right.png"), QIcon::Normal);
    forwardIcon.addPixmap(QPixmap("icons:right_disabled.png"), QIcon::Disabled);
    historyForwardAction->setIcon(forwardIcon);
    historyForwardAction->setToolTip(tr("Go forward in history"));
    addAction(historyForwardAction);
    connect(historyForwardAction, &QAction::triggered, this, [=]() {
        if (activeTabView)
            activeTabView->forward();
    });


    /**
     * Reload button
     */
    stopReloadAction = new QAction(this);
    stopReloadAction->setEnabled(false);
    QList<QKeySequence> reloadShortcuts = QKeySequence::keyBindings(QKeySequence::Refresh);
    for (auto it = reloadShortcuts.begin(); it != reloadShortcuts.end();) {
        if (((*it)[0].key() & Qt::Key_unknown) == Qt::Key_Backspace)
            it = reloadShortcuts.erase(it);
        else
            ++it;
    }
    reloadShortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_R));
    stopReloadAction->setShortcuts(reloadShortcuts);

    connect(stopReloadAction, &QAction::triggered, this, [=]() {
        if (activeTabView && activeTabView->pageView())
            activeTabView->pageView()->reload();
    });
    stopReloadAction->setIcon(QIcon(QStringLiteral("icons:reload.png")));
    addAction(stopReloadAction);

    /**
    * URL
    */
    urlLineEdit = new QLineEdit(this);
    favAction = new QAction(this);
    urlLineEdit->addAction(favAction, QLineEdit::LeadingPosition);
    urlLineEdit->setClearButtonEnabled(true);
    addWidget(urlLineEdit);


    /**
     * Menu
     */
    QToolButton *menuButton = new QToolButton();
    menuButton->setObjectName("MenuButton");
    menuButton->setPopupMode(QToolButton::InstantPopup);
    menuButton->setToolTip(tr("Menu"));
    addWidget(menuButton);

    QMenu *mainMenu=new QMenu(menuButton);
    menuButton->setMenu(mainMenu);

    // New tab
    newTabAction = new QAction("New tab", this);
    newTabAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    mainMenu->addAction(newTabAction);

    // New window
    newWindowAction = new QAction("New window", this);
    newWindowAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    mainMenu->addAction(newWindowAction);

    // New incognito window
    newIncognitoAction = new QAction("New incognito window", this);
    newIncognitoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N));
    mainMenu->addAction(newIncognitoAction);

    mainMenu->addSeparator();

    // History
    historyAction = new QAction("History", this);
    historyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    mainMenu->addAction(historyAction);
    connect(historyAction, &QAction::triggered, this, [this](){
        TabView *tabView = (qobject_cast<TabWidget*>(this->parent()))->createActiveTab();
        tabView->setUrl(INTERNAL_URL_SCHEME + "://" + BrowserPaths::historyManagerName);
    });

    // Downloads
    downloadsAction = new QAction("Downloads", this);
    downloadsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_J));
    mainMenu->addAction(downloadsAction);
    connect(downloadsAction, &QAction::triggered, this, [this](){
        TabView *tabView = (qobject_cast<TabWidget*>(this->parent()))->createActiveTab();
        tabView->setUrl(INTERNAL_URL_SCHEME + "://" + BrowserPaths::downloadManagerName);
    });


    mainMenu->addSeparator();

    QAction *zoomIn = mainMenu->addAction(tr("Zoom &In"));
    zoomIn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal));
    connect(zoomIn, &QAction::triggered, this, [=]() {
        if (activeTabView && activeTabView->pageView())
            activeTabView->pageView()->setZoomFactor(activeTabView->pageView()->zoomFactor() + 0.1);
    });

    QAction *zoomOut = mainMenu->addAction(tr("Zoom &Out"));
    zoomOut->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
    connect(zoomOut, &QAction::triggered, this, [=]() {
        if (activeTabView && activeTabView->pageView())
            activeTabView->pageView()->setZoomFactor(activeTabView->pageView()->zoomFactor() - 0.1);
    });

    QAction *resetZoom = mainMenu->addAction(tr("Reset &Zoom"));
    resetZoom->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(resetZoom, &QAction::triggered, this, [=]() {
        if (activeTabView && activeTabView->pageView())
            activeTabView->pageView()->setZoomFactor(1.0);
    });

    mainMenu->addSeparator();

    QAction *devTools = mainMenu->addAction(tr("Developer tools"));
    devTools->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
    connect(devTools, &QAction::triggered, this, [=]() {
        if (activeTabView && activeTabView->pageView())
            activeTabView->pageView()->toggleDevTools();
    });

    mainMenu->addSeparator();

    // Settings
    settingsAction = new QAction(tr("Settings"), this);
    mainMenu->addAction(settingsAction);
    connect(settingsAction, &QAction::triggered, this, [this](){
        TabView *tabView = (qobject_cast<TabWidget*>(this->parent()))->createActiveTab();
        tabView->setUrl(INTERNAL_URL_SCHEME + "://" + BrowserPaths::settingsManagerName);
    });

    mainMenu->addSeparator();

    // About
    auto aboutAction = new QAction("About QmlBrowser", this);
    connect(aboutAction, &QAction::triggered, [=](){
        AboutDialog* dialog = new AboutDialog(parent->window());
        dialog->open();
    });
    mainMenu->addAction(aboutAction);


    mainMenu->addSeparator();

    // Exit
    auto exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, [=](){
        QCoreApplication::quit();
    });
    mainMenu->addAction(exitAction);


    auto reloadStyleAction = new QAction("Reload Browser Styles", this);
    connect(reloadStyleAction, &QAction::triggered, this, [=](){
        QFile file("styles.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        ((QApplication*) QApplication::instance())->setStyleSheet(styleSheet);
    });
    mainMenu->addAction(reloadStyleAction);


    m_closeTabShortcut =  new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_W), this);
    connect(m_closeTabShortcut, &QShortcut::activated, this, [=] {
        TabWidget *w = qobject_cast<TabWidget*>(this->parent());
        int index = w->currentIndex();
        w->closeTab(index);
        if(index == 0) {
            QApplication::quit();
        }
    });

}


void NavigationBar::switchTab(TabView *tabView)
{
    if(activeTabView) {
        disconnect(activeTabView, &TabView::urlChanged, nullptr, nullptr);
    }

    if(tabView) {
        activeTabView = tabView;
        connect(activeTabView, &TabView::urlChanged, this, &NavigationBar::changeUrl);
        changeUrl(activeTabView->getCurrentUrl());
        urlLineEdit->setFocus();
    }
}


void NavigationBar::changeUrl(const QUrl url)
{
    historyBackAction->setEnabled(activeTabView->historyBack.count() > 0);
    historyForwardAction->setEnabled(activeTabView->historyForward.count() > 0);
    urlLineEdit->setText(url.toString());
    stopReloadAction->setEnabled(true);

}
