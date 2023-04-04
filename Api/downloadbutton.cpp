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
#include "downloadbutton.h"
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QStylePainter>
#include <QStyleOptionButton>
#include "tools.h"

DownloadButton::DownloadButton(DownloadItem *downloadItem, QWidget *parent)
    : QToolButton{parent},
      m_downloadItem(downloadItem)
{
    setText(downloadItem->downloadFileName());
    setPopupMode(QToolButton::InstantPopup);
    setToolTip(downloadItem->downloadFileName());

    QMenu *popMenu=new QMenu(this);
    setMenu(popMenu);

    // Open file
    openAction = new QAction("Open", this);
    popMenu->addAction(openAction);

    connect(openAction, &QAction::triggered, [this](){
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_downloadItem->path()));
    });

    // Cancel download
    cancelAction = new QAction("Cancel", this);
    popMenu->addAction(cancelAction);

    connect(cancelAction, &QAction::triggered, [this](){
        m_downloadItem->cancel();
        m_underInfo = tr("canceled");
        repaint();
    });

    connect(downloadItem, &DownloadItem::downloadProgress, this, &DownloadButton::downloadProgress);
    connect(downloadItem, &DownloadItem::stateChanged, [this](DownloadItem::DownloadState state) {
        cancelAction->setEnabled(state == DownloadItem::DownloadInProgress);
    });

}

void DownloadButton::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_underInfo = Tools::bytesToText(bytesReceived);
    m_totalInfo = Tools::bytesToText(bytesTotal);
    repaint();
}

void DownloadButton::paintEvent(QPaintEvent *p)
{
    QToolButton::paintEvent(p);
    if(nullptr != m_underInfo) {
        QPainter paint(this);
        paint.setFont(QFont(paint.font().family(), paint.font().pointSize()-1));
        paint.drawText(rect(),Qt::AlignCenter | Qt::AlignBottom, m_underInfo);
    }

}
