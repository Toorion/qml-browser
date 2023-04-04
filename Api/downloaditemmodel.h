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
#ifndef DOWNLOADITEMMODEL_H
#define DOWNLOADITEMMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "downloaditem.h"
#include "Api_global.h"

class API_EXPORT DownloadItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DownloadItemModel(QObject *parent = 0);
    DownloadItemModel(QList<DownloadItem*> items, QObject *parent = 0);

    enum ItemRoles {
        IndexRole = Qt::UserRole + 1,
        IdRole,
        NameRole,
        UrlRole,
        PathRole,
        ReceivedRole
    };

    void addDownloadItem(DownloadItem *item);

    Q_INVOKABLE bool removeDownloadItem(qint64 const index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

signals:

    void removeItem(const QString &id);

protected:
    QHash<int,QByteArray> roleNames() const override;

private:
    QList<DownloadItem*> m_items;
};

#endif // DOWNLOADITEMMODEL_H
