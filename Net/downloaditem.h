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
#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H

#include <QObject>
#include <QScopedPointer>
#include <QUrl>
#include <QNetworkReply>
#include "Net_global.h"

class DownloadItemPrivate;

class NET_EXPORT DownloadItem : public QObject
{
    Q_OBJECT
public:

    enum DownloadState {
        DownloadRequested,
        DownloadInProgress,
        DownloadCompleted,
        DownloadCancelled,
        DownloadInterrupted,
    };
    Q_ENUM(DownloadState)

    DownloadItem(const QUrl url, QObject *parent = Q_NULLPTR);
    ~DownloadItem();

    Q_INVOKABLE virtual DownloadItem::DownloadState state() const;

    Q_INVOKABLE virtual QString downloadDirectory() const;
    void setDownloadDirectory(QString directory);
    Q_INVOKABLE virtual QString downloadFileName() const;
    virtual void setDownloadFileName(QString fileName);
    Q_INVOKABLE bool isFinished() const;

    Q_INVOKABLE QString path() const;

    Q_INVOKABLE QUrl url() const;

    Q_INVOKABLE QDateTime startDownloadAt() const;

    Q_INVOKABLE qint64 receivedBytes() const;

    QString id() const;

    bool openFile();

    void setReply(QNetworkReply *reply);

    void setReceivedBytes(const qint64 bytes);

    void setId(const QString id);

public Q_SLOTS:
    virtual void accept();
    virtual void cancel();
    virtual void pause();
    virtual void resume();

    void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

Q_SIGNALS:
    void finished();
    void stateChanged(DownloadItem::DownloadState state);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void isPausedChanged(bool isPaused);

    void callAccept();
    void callCancel();
    void callPause();
    void callResume();


private:

    void doCancel();

    QScopedPointer<DownloadItemPrivate> d_ptr;

    Q_DECLARE_PRIVATE(DownloadItem)
    Q_DISABLE_COPY(DownloadItem)

private slots:

    // Slot for gradual reading of downloaded data
    void onReadyRead();
    // Slot for processing request completion
    void onReply();


};

Q_DECLARE_METATYPE(DownloadItem*)

#endif // DOWNLOADITEM_H
