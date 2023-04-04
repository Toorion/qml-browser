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
#include "downloaditemmodel.h"
#include "tools.h"


DownloadItemModel::DownloadItemModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

DownloadItemModel::DownloadItemModel(QList<DownloadItem *> items, QObject *parent)
    : QAbstractListModel{parent}
{
    m_items = items;
}


QHash<int, QByteArray> DownloadItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[UrlRole] = "url";
    roles[PathRole] = "path";
    roles[ReceivedRole] = "received";
    return roles;
}


void DownloadItemModel::addDownloadItem(DownloadItem *item)
{
    QModelIndex idx = QModelIndex();
    beginInsertRows(idx, rowCount(), rowCount());
    m_items << item;
    endInsertRows();
    int row = m_items.count() - 1;
    connect(item, &DownloadItem::downloadProgress, [this, row](qint64 bytesReceived, qint64 bytesTotal) {
        (void)bytesReceived;
        (void)bytesTotal;
        emit dataChanged(index(row), index(row));
//        layoutChanged(); - same working
    });
}

bool DownloadItemModel::removeDownloadItem(const qint64 index)
{
    if(index >= 0 && index < m_items.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        emit removeItem(m_items.at(index)->id());
        m_items.removeAt(index);
        endRemoveRows();
        layoutChanged(); // required for reindexing items
        return true;
    }
    return false;
}

int DownloadItemModel::rowCount(const QModelIndex & parent) const {
    if(parent.isValid()) return 0;
    return m_items.count();
}

QVariant DownloadItemModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    DownloadItem *item = m_items[index.row()];
    if (role == IndexRole)
        return index.row();
    else if (role == IdRole)
        return item->id();
    else if (role == NameRole)
        return item->downloadFileName();
    else if (role == UrlRole)
        return item->url().toString();
    else if (role == PathRole)
        return item->path();
    else if (role == ReceivedRole)
        return Tools::bytesToText(item->receivedBytes());
    return QVariant();
}
