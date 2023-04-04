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
#include "downloaditem.h"
#include "downloaditem_p.h"
#include <QDir>

QT_BEGIN_NAMESPACE

DownloadItemPrivate::DownloadItemPrivate(const QUrl &url)
    : downloadFinished(false)
    , downloadId(-1)
    , downloadState(DownloadItem::DownloadCancelled)
//    , savePageFormat(DownloadItem::MimeHtmlSaveFormat)
//    , type(DownloadItem::Attachment)
//    , interruptReason(DownloadItem::NoReason)
    , downloadUrl(url)
    , downloadPaused(false)
    , totalBytes(-1)
    , receivedBytes(0)
{
}

DownloadItemPrivate::~DownloadItemPrivate()
{
}

void DownloadItemPrivate::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    receivedBytes = bytesReceived;
    totalBytes = bytesTotal;
}

DownloadItem::DownloadItem(const QUrl url, QObject *parent)
    : QObject(parent)
    , d_ptr(new DownloadItemPrivate(url))
{
    Q_D(DownloadItem);
    d->downloadState = DownloadItem::DownloadRequested;
    d->startDownloadAt = QDateTime::currentDateTime();
}

DownloadItem::~DownloadItem()
{
    if (!isFinished())
         doCancel();
}

DownloadItem::DownloadState DownloadItem::state() const
{
    Q_D(const DownloadItem);
    return d->downloadState;
}

QString DownloadItem::downloadDirectory() const
{
    Q_D(const DownloadItem);
    return d->downloadDirectory;
}

void DownloadItem::setDownloadDirectory(QString directory)
{
    Q_D(DownloadItem);
    if (d->downloadState != DownloadItem::DownloadRequested) {
        qWarning("Setting the download directory is not allowed after the download has been accepted.");
        return;
     }
    if (!directory.isEmpty() && d->downloadDirectory != directory)
        d->downloadDirectory = directory;

}

QString DownloadItem::downloadFileName() const
{
    Q_D(const DownloadItem);
    return d->downloadFileName;
}

void DownloadItem::setDownloadFileName(QString fileName)
{
    Q_D(DownloadItem);
    if (d->downloadState != DownloadItem::DownloadRequested) {
       qWarning("Setting the download file name is not allowed after the download has been accepted.");
       return;
    }
    if (!fileName.isEmpty())
        d->downloadFileName = fileName;
}

bool DownloadItem::isFinished() const
{
    Q_D(const DownloadItem);
    return d->downloadFinished;
}

QString DownloadItem::path() const
{
    return QDir(downloadDirectory()).absoluteFilePath(downloadFileName());
}

QUrl DownloadItem::url() const
{
    Q_D(const DownloadItem);
    return d->downloadUrl;
}

QDateTime DownloadItem::startDownloadAt() const
{
    Q_D(const DownloadItem);
    return d->startDownloadAt;
}

qint64 DownloadItem::receivedBytes() const
{
    Q_D(const DownloadItem);
    return d->receivedBytes;
}

QString DownloadItem::id() const
{
    Q_D(const DownloadItem);
    return d->id;
}

bool DownloadItem::openFile()
{
    Q_D(DownloadItem);
    d->file = new QFile(path());

    // Trying to open the file
    if (!d->file->open(QIODevice::WriteOnly))
    {
        delete d->file;
        d->file = nullptr;
        return false;
    }
    return true;
}

void DownloadItem::setReply(QNetworkReply *reply)
{
    Q_D(DownloadItem);
    d->reply = reply;

    connect(d->reply, &QNetworkReply::readyRead, this, &DownloadItem::onReadyRead);
    connect(d->reply, &QNetworkReply::downloadProgress, this, &DownloadItem::updateDownloadProgress);
    connect(d->reply, &QNetworkReply::finished, this, &DownloadItem::onReply);
}

void DownloadItem::setReceivedBytes(const qint64 bytes)
{
    Q_D(DownloadItem);
    d->receivedBytes = bytes;
}

void DownloadItem::setId(const QString id)
{
    Q_D(DownloadItem);
    d->id = id;
}

void DownloadItem::onReadyRead()
{
    Q_D(DownloadItem);
    // If there is data and the file is open
    if (d->file)
    {
        // write them to a file
        d->file->write(d->reply->readAll());
    }
}

void DownloadItem::onReply()
{
    Q_D(DownloadItem);
    if (d->file)
    {
        // by completion of the request
        if (d->reply->error() == QNetworkReply::NoError)
        {
            // save file
            d->file->flush();
            d->file->close();
            d->updateProgress(d->totalBytes, d->totalBytes);
            emit downloadProgress(d->totalBytes, d->totalBytes);

        } else {
            // Or delete it in case of error
            d->file->remove();
        }
        delete d->file;
        d->file = nullptr;
    }
    d->reply->close();
    d->reply->deleteLater();
    emit finished();
}

void DownloadItem::accept()
{
    Q_D(DownloadItem);

    if (d->downloadState != DownloadItem::DownloadRequested)
            return;

    d->downloadState = DownloadItem::DownloadInProgress;
    emit callAccept();
    emit stateChanged(d->downloadState);
}

void DownloadItem::cancel()
{
    doCancel();
}

void DownloadItem::doCancel()
{
    Q_D(DownloadItem);

    if (d->downloadState != DownloadItem::DownloadInProgress)
            return;

    if (d->reply)
    {
        d->reply->abort();
    }

    emit callCancel();
    d->downloadState = DownloadItem::DownloadCancelled;
    emit stateChanged(d->downloadState);
}

void DownloadItem::pause()
{
    Q_D(DownloadItem);

    if (d->downloadState != DownloadItem::DownloadInProgress)
            return;

    emit callPause();
    d->downloadPaused = true;
}

void DownloadItem::resume()
{
    Q_D(DownloadItem);

    if (!d->downloadPaused)
            return;

    emit callResume();
    d->downloadPaused = false;
}

void DownloadItem::updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_D(DownloadItem);
    d->updateProgress(bytesReceived, bytesTotal);
    emit downloadProgress(bytesReceived, bytesTotal);
}


QT_END_NAMESPACE

#include "moc_downloaditem.cpp"
