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
#ifndef DOWNLOADITEMPROXY_H
#define DOWNLOADITEMPROXY_H

#include <downloaditem.h>
#include <QWebEngineDownloadRequest>

class DownloadItemProxy : public DownloadItem
{
    Q_OBJECT
public:
    explicit DownloadItemProxy(QWebEngineDownloadRequest *item, QObject *parent = nullptr);

    void sync();

    DownloadState state() const override;

    QString downloadDirectory() const override;

    QString downloadFileName() const override;
    void setDownloadFileName(QString fileName) override;

public Q_SLOTS:
    void accept() override;
    void cancel() override;
    void pause() override;
    void resume() override;
    void receivedBytesChanged();

private:
    QWebEngineDownloadRequest *m_item;
};

#endif // DOWNLOADITEMPROXY_H
