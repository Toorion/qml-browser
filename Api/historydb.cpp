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
#include "historydb.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QSqlError>

Q_GLOBAL_STATIC(HistoryDb, gs_history_db)

HistoryDb::HistoryDb()
    : AppDb()
{
    m_dbName = QLatin1String("history.db");
    m_tableName = QLatin1String("url");
    connect();
}

bool HistoryDb::create()
{
    QSqlQuery query(m_db);
    // Linux path 4095 max
    if(!query.exec( "CREATE TABLE " + m_tableName + " ("
            "id INTEGER PRIMARY KEY,"
            "url VARCHAR(2048) NOT NULL,"
            "type int NOT NULL,"
            "title VARCHAR(1024) NOT NULL,"
            "icon_url VARCHAR(2048),"
            "added INTEGER NOT NULL"
            " )"
        )) {
        return queryError("create", query.lastError().text());
    }
    return true;
}


int HistoryDb::insert(const HistoryItem &item)
{
    HistoryDb *p = gs_history_db();

    QSqlQuery query(p->m_db);

    query.prepare("INSERT INTO " + p->m_tableName + " ( url, type, title, icon_url, added ) "
                      "VALUES (:Url, :Type, :Title, :IconUrl, :Added)");
    query.bindValue(":Url",   item.url());
    query.bindValue(":Type",  item.type());
    query.bindValue(":Title",  item.title().isNull() ? QString("") : item.title());
    query.bindValue(":IconUrl",  item.iconUrl());
    query.bindValue(":Added", item.added().toSecsSinceEpoch());

    if(!query.exec()){
        return p->queryError("insert", query.lastError().text());
    }

    QVariant id = query.lastInsertId();
    Q_ASSERT(id.isValid() && !id.isNull());

    return id.toInt();
}

bool HistoryDb::update(const HistoryItem &item)
{
    HistoryDb *p = gs_history_db();

    QSqlQuery query(p->m_db);

    query.prepare("UPDATE " + p->m_tableName + " SET url=:Url, type=:Type, title=:Title, icon_url=:IconUrl, added=:Added WHERE id=:Id");
    query.bindValue(":Id",   item.id());
    query.bindValue(":Url",   item.url());
    query.bindValue(":Type",  item.type());
    query.bindValue(":Title",  item.title());
    query.bindValue(":IconUrl",  item.iconUrl());
    query.bindValue(":Added", item.added().toSecsSinceEpoch());

    if(!query.exec()){
        return p->queryError("update", query.lastError().text());
    }

    return true;
}

bool HistoryDb::remove(const int id)
{
    HistoryDb *p = gs_history_db();
    return p->removeRecord(id);
}

QList<HistoryItem *> HistoryDb::list()
{
    HistoryDb *p = gs_history_db();

    QList <HistoryItem*> list;
    QSqlQuery query("SELECT id, url, type, title, icon_url, added FROM " + p->m_tableName + " ORDER BY added DESC", p->m_db);
    while (query.next()) {
        // Url, Type
        auto item = new HistoryItem(query.value(1).toString(), query.value(2).toInt(), query.value(3).toString(), query.value(4).toString());
        // Id
        item->setId(query.value(0).toString());
        auto time = new QDateTime();
        time->setSecsSinceEpoch(query.value(5).toInt());
        item->setAdded(*time);
        list << item;
    }
    return list;
}
