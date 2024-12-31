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
#include "appdb.h"
#include "apppaths.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>

QMap<QLatin1String,QSqlDatabase*> AppDb::m_dbs = {};

AppDb::AppDb() : QObject()
{

}

const QString AppDb::dbFullName()
{
    return QDir(AppPaths::dbPath()).filePath(m_dbName);
}

const QVariantMap AppDb::queryToMap(QSqlQuery *query)
{
    QVariantMap map;
    for (int i = 0; i < query->record().count(); i++) {
        QString fieldName = query->record().fieldName(i);
        QVariant value = query->value(i);
        map.insert(fieldName, value);
    }
    return map;
}

bool AppDb::connect()
{
    bool exists = QFile(dbFullName()).exists();
    if(open()) {
        if(!exists) {
            return create();
        }
        return true;
    }
    return false;
}

bool AppDb::open()
{
    if (m_dbs.contains(m_dbName)) {
        m_db = *m_dbs.value(m_dbName);
        return true;
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_dbName + "Connection");
    m_db.setDatabaseName(dbFullName());
    bool isOpen = m_db.open();
    if(!isOpen) {
        queryError("Open", m_db.lastError().text());
    }
    m_dbs.insert(m_dbName, &m_db);
    return isOpen;
}

void AppDb::close()
{
    m_db.close();
}

bool AppDb::queryError(const QString command, const QString error)
{
    return queryError(m_tableName, command, error);
}

bool AppDb::queryError(const QLatin1String tableName, const QString command, const QString error)
{
    qWarning("DB Error, %s", dbFullName().toLocal8Bit().data());
    qWarning("Table: %s", tableName.data());
    qWarning("Command: %s", command.toLocal8Bit().data());
    qWarning("With error: %s", error.toLocal8Bit().data());
    return false;
}

bool AppDb::removeRecord(const int id)
{
    return removeRecord(m_tableName, id);
}

bool AppDb::removeRecord(const QLatin1String tableName, const int id)
{
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM " + tableName + " WHERE id=:id");
    query.bindValue(":id", id);

    if(!query.exec()){
        return queryError(tableName, "remove", query.lastError().text());
    }
    return true;
}

