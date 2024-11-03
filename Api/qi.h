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
#include "downloadmanagerwidget.h"
#include "historyitemmodel.h"
#include <QSettings>
#include <QQmlPropertyMap>
#include "Api_global.h"
#include "bookmarkitemmodel.h"
#include "bookmarklinkmodel.h"

class API_EXPORT Qi : public QObject
{
    Q_OBJECT
public:
    explicit Qi(QObject *parent = nullptr);

    Q_INVOKABLE DownloadItemModel *downloadHistoryModel();
    Q_INVOKABLE HistoryItemModel *visitHistoryModel();
    Q_INVOKABLE BookmarkItemModel *bookmarkModel();
    Q_INVOKABLE BookmarkLinkModel *bookmarkLinkModel();
    Q_INVOKABLE QQmlPropertyMap *settingsModel();

    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString progressStatus READ progressStatus NOTIFY progressChanged)
    Q_PROPERTY(QQmlPropertyMap * progressInfo READ progressInfo NOTIFY progressInfoChanged)

    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

    void setDownloadManagerWidget(DownloadManagerWidget *downloadManagerWidget) {
        m_downloadManagerWidget = downloadManagerWidget;
    };

    DownloadManagerWidget *downloadManagerWidget() {
        return m_downloadManagerWidget;
    }

    void setProgress(int progress, QString status) {
        m_progress = progress;
        m_progressStatus = status;
        emit progressChanged();
    }

    int progress() {
        return m_progress;
    }

    QString progressStatus() {
        return m_progressStatus;
    }

    void setProgressInfoProperty(QString key, QVariant value) {
        m_progressInfo->insert(key, value);
        emit progressInfoChanged();
    }

    QQmlPropertyMap *progressInfo() {
        return m_progressInfo;
    }

    void setError(QString error) {
        m_error = error;
        emit errorChanged();
    }

    QString error() {
        return m_error;
    }

    QSettings static *settings;

signals:

    void progressChanged();
    void progressInfoChanged();
    void errorChanged();

private:

    DownloadManagerWidget *m_downloadManagerWidget;

    int m_progress = 0;

    QString m_progressStatus;

    QQmlPropertyMap *m_progressInfo;

    QString m_error;

};

#endif // QI_H
