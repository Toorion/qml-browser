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
#ifndef HISTORYITEMMODEL_H
#define HISTORYITEMMODEL_H

#include <QAbstractListModel>
#include "historyitem.h"
#include "Api_global.h"

class API_EXPORT HistoryItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    static HistoryItemModel& instance(QObject *parent = nullptr);
    explicit HistoryItemModel(QObject *parent = nullptr);
    ~HistoryItemModel();

    void fill(QList<HistoryItem*> items);

    enum ItemRoles {
        IdxRole = Qt::UserRole + 1,
        IdRole,
        UrlRole,
        TitleRole,
        IconUrlRole,
        AddedRole,
        TypeRole,
        HostRole
    };

    void addHistoryItem(HistoryItem *item);

    void updateHistoryItem(HistoryItem *item);

    Q_INVOKABLE bool removeHistoryItem(qint64 const index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QVariantMap get(int row) const;

protected:
    QHash<int,QByteArray> roleNames() const override;

private:
    HistoryItemModel(const HistoryItemModel&other);
    HistoryItemModel& operator=(const HistoryItemModel&other);

    QList<HistoryItem*> m_items;

};

#endif // HISTORYITEMMODEL_H
