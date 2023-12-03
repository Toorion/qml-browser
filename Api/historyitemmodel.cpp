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
#include "historyitemmodel.h"
#include "historydb.h"
#include <QQmlEngine>
#include "urlhelper.h"

HistoryItemModel &HistoryItemModel::instance(QObject *parent)
{
    static HistoryItemModel instance(parent);
    return instance;
}

HistoryItemModel::~HistoryItemModel()
{

}

HistoryItemModel::HistoryItemModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void HistoryItemModel::fill(QList<HistoryItem *> items)
{
    m_items = items;
}


QHash<int, QByteArray> HistoryItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdxRole] = "idx";
    roles[IdRole] = "id";
    roles[UrlRole] = "url";
    roles[TitleRole] = "title";
    roles[IconUrlRole] = "iconUrl";
    roles[AddedRole] = "added";
    roles[TypeRole] = "type";
    roles[HostRole] = "host";
    return roles;
}


void HistoryItemModel::addHistoryItem(HistoryItem *item)
{
    if(item->url().scheme() == INTERNAL_URL_SCHEME) {
        return;
    }
    int id = HistoryDb::insert(*item);
    item->setId(QString::number(id));
    QModelIndex idx = QModelIndex();
    beginInsertRows(idx, 0, 0);
    m_items.prepend(item);
    endInsertRows();
}

void HistoryItemModel::updateHistoryItem(HistoryItem *item)
{
    HistoryDb::update(*item);
    for( int i=0; i<m_items.count(); ++i) {
        if(item->id() == m_items.at(i)->id()) {
            QModelIndex topLeft = createIndex(0,0);
            emit dataChanged(topLeft, topLeft);
            break;
        }
    }
}

bool HistoryItemModel::removeHistoryItem(const qint64 index)
{
    if(index >= 0 && index < m_items.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        HistoryDb::remove(m_items.at(index)->id().toInt());
        m_items.removeAt(index);
        endRemoveRows();
        emit layoutChanged(); // required for reindexing items
        return true;
    }
    return false;
}

int HistoryItemModel::rowCount(const QModelIndex & parent) const {
    if(parent.isValid()) return 0;
    return m_items.count();
}

QVariant HistoryItemModel::data(const QModelIndex & index, int role) const {
    QString v("");

    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    HistoryItem *item = m_items[index.row()];
    if (role == IdxRole)
        return index.row();
    else if (role == IdRole)
        return item->id();
    else if (role == UrlRole)
        return item->url().toString();
    else if (role == TitleRole)
        return item->title();
    else if (role == IconUrlRole)
        return item->iconUrl().toString();
    else if (role == AddedRole)
        return item->added();
    else if (role == TypeRole)
        return item->type();
    else if (role == HostRole)
        return item->url().host();
    return QVariant();
}

QVariantMap HistoryItemModel::get(int row) const {
    QVariantMap map;
    if(row < 0 || row >= m_items.count())
        return map;

    map["id"] = m_items[row]->id();
    map["url"] = m_items[row]->url().toString();
    map["title"] = m_items[row]->title();
    map["iconUrl"] = m_items[row]->iconUrl().toString();
    map["added"] = m_items[row]->added();
    map["type"] = m_items[row]->type();
    map["host"] = m_items[row]->url().host();

    return map;
}
