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
#ifndef QI_H
#define QI_H

#include <QObject>
#include "downloaditemmodel.h"
#include <QVariant>
#include "accessrights.h"
#include "downloadmanagerwidget.h"
#include "historyitemmodel.h"
#include <QSettings>
#include <QQmlPropertyMap>
#include "Api_global.h"

class API_EXPORT Qi : public QObject
{
    Q_OBJECT
public:
    explicit Qi(QObject *parent = nullptr);

    Q_INVOKABLE DownloadItemModel *downloadHistoryModel();
    Q_INVOKABLE HistoryItemModel *visitHistoryModel();
    Q_INVOKABLE QQmlPropertyMap *settingsModel();

    void setDownloadManagerWidget(DownloadManagerWidget *downloadManagerWidget) {
        m_downloadManagerWidget = downloadManagerWidget;
    };

    DownloadManagerWidget *downloadManagerWidget() {
        return m_downloadManagerWidget;
    }

    QSettings static *settings;

signals:

private:

    DownloadManagerWidget *m_downloadManagerWidget;

};

#endif // QI_H
