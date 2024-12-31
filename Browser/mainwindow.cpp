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
#include "mainwindow.h"
#include <QVBoxLayout>
#include "tabwidget.h"
#include <QAction>
#include <QLineEdit>
#include "navigationbar.h"
#include <tabview.h>
#include <QWebEngineProfile>
#include <QApplication>
#include <QMessageBox>
#include "downloaditem.h"
#include "downloaditemproxy.h"
#include "browserpaths.h"
#include <QScreen>
#include "urlhelper.h"
#include <QPixmap>
#include <QPainterPath>
#include <QRect>

MainWindow::MainWindow(QWebEngineProfile *profile)
    : m_profile(profile)
{
    setObjectName("QUI-BrowserWindow");

    QPixmap qbqb3("icons:qbqb3.svg");
    setWindowIcon(QIcon(qbqb3));
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::ClickFocus);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    tabWidget = new TabWidget(m_profile, this);
    tabWidget->setMovable(true);
    tabWidget->setTabsClosable(true);
    tabWidget->setElideMode(Qt::TextElideMode::ElideRight);

    layout->addWidget(tabWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->show();

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    navigationBar = new NavigationBar(tabWidget);
    tabWidget->setToolBar(navigationBar);

    connect(tabWidget, &TabWidget::titleChanged, this, &MainWindow::handleWebViewTitleChanged);

    connect(tabWidget, &TabWidget::currentChanged, this, [=]() {
        navigationBar->connectTab(tabWidget->currentView());
    });

    connect(navigationBar->urlLineEdit, &QLineEdit::returnPressed, this, [=]() {
        tabWidget->currentView()->loadUrl(navigationBar->urlLineEdit->text());
    });

    connect(tabWidget, &TabWidget::plusClicked, tabWidget, &TabWidget::createActiveTab);

    connect(navigationBar->newTabAction, &QAction::triggered, tabWidget, &TabWidget::createActiveTab);

    tabWidget->createActiveTab();

    m_downloadManagerWidget = new DownloadManagerWidget(this);
    connect(m_downloadManagerWidget, &DownloadManagerWidget::closeManager, this, &MainWindow::closeDownloadManager);
    connect(m_downloadManagerWidget, &DownloadManagerWidget::showAllTriggered, this, [=] () {
        TabView *tab = tabWidget->createActiveTab();
        tab->loadUrl(INTERNAL_URL_SCHEME + "://" + BrowserPaths::downloadManagerName);
    });
    layout->addWidget(m_downloadManagerWidget);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setMaximumHeight(1);
    m_progressBar->setTextVisible(false);
    connect(navigationBar, &NavigationBar::loadProgress, this, &MainWindow::handleLoadProgress);
    layout->addWidget(m_progressBar);
}


MainWindow::~MainWindow()
{
}


QSize MainWindow::sizeHint() const
{
    QRect desktopRect = QApplication::primaryScreen()->geometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

void MainWindow::htmlDownloadRequested(QWebEngineDownloadRequest *item)
{
    DownloadItemProxy *downloadItemProxy = new DownloadItemProxy(item);
    if(!m_downloadManagerWidget->isVisible()) {
      m_downloadManagerWidget->setVisible(true);
    }
    m_downloadManagerWidget->downloadRequested(qobject_cast<DownloadItem*>(downloadItemProxy));
}

void MainWindow::closeDownloadManager()
{
    m_downloadManagerWidget->setVisible(false);
}

void MainWindow::handleLoadProgress(int progress)
{
    m_progressBar->setValue(progress);
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (tabWidget->count() > 2) {
        int ret = QMessageBox::warning(this, tr("Confirm close"),
                                       tr("Are you sure you want to close the window ?\n"
                                          "There are %1 tabs open.").arg(tabWidget->count() - 1),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    event->accept();
    deleteLater();
}

void MainWindow::handleWebViewTitleChanged(const QString &title)
{
    QString suffix = m_profile->isOffTheRecord()
        ? tr("Qml Browser (Incognito)")
        : tr("Qml Browser");

    if (title.isEmpty())
        setWindowTitle(suffix);
    else
        setWindowTitle(title + " - " + suffix);
}



