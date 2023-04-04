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
#include "downloaddb.h"
#include <QSqlQuery>
#include <QVariant>
#include <downloaditem.h>
#include <QFile>
#include <QSqlError>

Q_GLOBAL_STATIC(DownloadDb, gs_download_db)

DownloadDb::DownloadDb()
    : AppDb()
{
    m_dbName = QLatin1String("download.db");
    m_tableName = QLatin1String("history");
    connect();
}

bool DownloadDb::create()
{
    QSqlQuery query(m_db);
    // Linux path 4095 max
    if(!query.exec( "CREATE TABLE " + m_tableName + " ("
            "id INTEGER PRIMARY KEY,"
            "url VARCHAR(2048) NOT NULL,"
            "path VARCHAR(4096) NOT NULL,"
            "added INTEGER NOT NULL"
            " )"
        )) {
        return queryError("create", query.lastError().text());
    }
    return true;
}


int DownloadDb::insert(const DownloadItem &item)
{
    DownloadDb *p = gs_download_db();

    QSqlQuery query(p->m_db);

    query.prepare("INSERT INTO " + p->m_tableName + " ( url, path, added ) "
                      "VALUES (:Url, :Path, :Added)");
    query.bindValue(":Url",   item.url());
    query.bindValue(":Path",  item.path());
    query.bindValue(":Added", item.startDownloadAt().toSecsSinceEpoch());

    if(!query.exec()){
        return p->queryError("insert", query.lastError().text());
    }

    QVariant id = query.lastInsertId();
    Q_ASSERT(id.isValid() && !id.isNull());
    return id.toInt();
}

bool DownloadDb::remove(const int id)
{
    DownloadDb *p = gs_download_db();
    return p->removeRecord(id);
}

QList<DownloadItem *> DownloadDb::list()
{
    DownloadDb *p = gs_download_db();

    QList <DownloadItem*> list;
    QSqlQuery query("SELECT id, url, path, added FROM " + p->m_tableName + " ORDER BY id", p->m_db);
    while (query.next()) {
        // Url
        auto item = new DownloadItem(query.value(1).toString());
        // Id
        item->setId(query.value(0).toString());
        // Path
        QUrl filePath(query.value(2).toString());
        item->setDownloadDirectory(filePath.adjusted(QUrl::RemoveFilename).path());
        item->setDownloadFileName(filePath.fileName());
        QFile file(query.value(2).toString());
        if(file.exists()) {
            item->setReceivedBytes(file.size());
        }
        list << item;
    }
    return list;
}
