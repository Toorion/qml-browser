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
#include "downloadmanagerwidget.h"
#include "downloaditem.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include "downloadbutton.h"
#include "downloaditemmodel.h"
#include "downloaddb.h"

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent) : QToolBar(parent)
{
    setVisible(false);
    setMovable(false);
    toggleViewAction()->setEnabled(false);

    setFixedHeight(60);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(spacer);

    showAllAction = new QAction(this);
    showAllAction->setText(tr("Show all"));
    addAction(showAllAction);
    (qobject_cast<QToolButton*>(widgetForAction(showAllAction)))->setObjectName("ShowAll");
    connect(showAllAction, &QAction::triggered, this, &DownloadManagerWidget::showAllTriggered);


    closeAction = new QAction(this);
    closeAction->setIcon(QIcon("icons:cross.png"));
    addAction(closeAction);
    (qobject_cast<QToolButton*>(widgetForAction(closeAction)))->setObjectName("Close");
    connect(closeAction, &QAction::triggered, this, &DownloadManagerWidget::closeManager);

    m_downloadItemModel = new DownloadItemModel(DownloadDb::list());
    connect(m_downloadItemModel, &DownloadItemModel::removeItem, [](const QString &id){
        DownloadDb::remove(id.toInt());
    });
}

void DownloadManagerWidget::downloadRequested(DownloadItem *downloadItem)
{
    Q_ASSERT(downloadItem && downloadItem->state() == DownloadItem::DownloadRequested);

    downloadItem->accept();

    downloadItem->setId(QString::number(DownloadDb::insert(*downloadItem)));

    m_downloadItemModel->addDownloadItem(downloadItem);

    add(new DownloadButton(downloadItem, this));
}


void DownloadManagerWidget::add(DownloadButton *downloadButton)
{
//    connect(downloadWidget, &DownloadWidget::removeClicked, this, &DownloadManagerWidget::remove);
//    m_itemsLayout->insertWidget(0, downloadWidget, 0, Qt::AlignTop);
//    if (m_numDownloads++ == 0)
//        m_zeroItemsLabel->hide();

    insertWidget(actions().at(0), downloadButton);
    m_numDownloads++;
}

void DownloadManagerWidget::remove(DownloadButton *downloadButton)
{
    layout()->removeWidget(downloadButton);
    downloadButton->deleteLater();
//    if (--m_numDownloads == 0)
//        m_zeroItemsLabel->show();
}
