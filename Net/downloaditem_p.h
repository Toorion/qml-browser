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
#ifndef DOWNLOADITEM_P_H
#define DOWNLOADITEM_P_H

#include "downloaditem.h"
#include <QUrl>
#include <private/qobject_p.h>
#include <QNetworkReply>
#include <QFile>
#include <QDateTime>


class DownloadItemPrivate : public QObjectPrivate {
    DownloadItem *q_ptr;

public:
    Q_DECLARE_PUBLIC(DownloadItem)
    DownloadItemPrivate(const QUrl &url);
    ~DownloadItemPrivate();

    bool downloadFinished;
    quint32 downloadId;
    DownloadItem::DownloadState downloadState;
//    DownloadItem::SavePageFormat savePageFormat;
//    DownloadItem::DownloadType type;
//    DownloadItem::DownloadInterruptReason interruptReason;
    QString downloadPath;
    const QUrl downloadUrl;
    QString mimeType;
    bool downloadPaused;
    QString suggestedFileName;
    QString downloadDirectory;
    QString downloadFileName;
    qint64 totalBytes;
    qint64 receivedBytes;

    QNetworkReply *reply;

    QFile *file;

    QDateTime startDownloadAt;

    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);

    void setFinished();

    QString id;

};

#endif // DOWNLOADITEM_P_H
