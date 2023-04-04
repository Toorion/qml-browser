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
#include "downloaditemproxy.h"
#include "downloaditem_p.h"

DownloadItemProxy::DownloadItemProxy(QWebEngineDownloadRequest *item, QObject *parent) : DownloadItem(item->url(), parent),
    m_item(item)
{
    connect(item, &QWebEngineDownloadRequest::receivedBytesChanged, this, &DownloadItemProxy::receivedBytesChanged);
    connect(item, &QWebEngineDownloadRequest::stateChanged, this, [=](QWebEngineDownloadRequest::DownloadState state){
        emit DownloadItem::stateChanged((DownloadItem::DownloadState) state);
    });
}

void DownloadItemProxy::sync()
{
    setDownloadFileName(m_item->downloadFileName());
}

DownloadItem::DownloadState DownloadItemProxy::state() const
{
    return (DownloadState)m_item->state();
}

QString DownloadItemProxy::downloadDirectory() const
{
    return m_item->downloadDirectory();
}

QString DownloadItemProxy::downloadFileName() const
{
    return m_item->downloadFileName();
}

void DownloadItemProxy::setDownloadFileName(QString fileName)
{
    m_item->setDownloadFileName(fileName);
}

void DownloadItemProxy::accept()
{
    m_item->accept();
}

void DownloadItemProxy::cancel()
{
    m_item->cancel();
}

void DownloadItemProxy::pause()
{
    m_item->pause();
}

void DownloadItemProxy::resume()
{
    m_item->resume();
}

void DownloadItemProxy::receivedBytesChanged()
{
    updateDownloadProgress(m_item->receivedBytes(),m_item->totalBytes());
}
