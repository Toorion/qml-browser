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
#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QObject>
#include <QUrl>
#include <QDateTime>
#include "Api_global.h"

class API_EXPORT HistoryItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString iconUrl READ iconUrlString CONSTANT)

public:

    explicit HistoryItem(const QUrl &url, const int &type, const QString &title, const QUrl &iconUrl, const QString &description);

    void setId(const int id) {
        m_id = id;
    };

    void setIconUrl(const QUrl iconUrl) {
        m_iconUrl = iconUrl;
    };

    void setAdded(const QDateTime added) {
        m_added = added;
    };

    int id() const {
        return m_id;
    }

    QUrl url() const {
        return m_url;
    }

    int type() const {
        return m_type;
    }

    QString title() const {
        return m_title;
    }

    QUrl iconUrl() const {
        return m_iconUrl;
    }

    QString description() const {
        return m_description;
    }

    QString iconUrlString() const {
        return m_iconUrl.toString();
    }

    QDateTime added() const {
        return m_added;
    }

private:

    int m_id;

    QUrl m_url;

    int m_type;

    QString m_title;

    QUrl m_iconUrl;

    QDateTime m_added;

    QString m_description;

};

#endif // HISTORYITEM_H
