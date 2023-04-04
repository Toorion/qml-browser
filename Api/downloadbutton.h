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
#ifndef DOWNLOADBUTTON_H
#define DOWNLOADBUTTON_H

#include <QToolButton>
#include "downloaditem.h"
#include <QAction>
#include "Api_global.h"

class API_EXPORT DownloadButton : public QToolButton
{
public:
    explicit DownloadButton(DownloadItem *downloadItem, QWidget *parent = nullptr);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);


protected:
    void paintEvent(QPaintEvent *p) override;
private:
    QAction *openAction;

    QAction *cancelAction;

    DownloadItem *m_downloadItem;

    QString m_underInfo = nullptr;

    QString m_totalInfo = nullptr;

};

#endif // DOWNLOADBUTTON_H
