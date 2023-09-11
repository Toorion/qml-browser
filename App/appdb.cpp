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

AppDb::AppDb() : QObject()
{

}

const QString AppDb::dbFullName()
{
    return QDir(AppPaths::dbPath()).filePath(m_dbName);
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
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_dbName + "Connection");
    m_db.setDatabaseName(dbFullName());
    bool isOpen = m_db.open();
    if(!isOpen) {
        queryError("Open", m_db.lastError().text());
    }
    return isOpen;
}

void AppDb::close()
{
    m_db.close();
}

bool AppDb::queryError(const QString command, const QString error)
{
    qWarning("DB Error, %s", dbFullName().toLocal8Bit().data());
    qWarning("Table: %s", m_tableName.data());
    qWarning("Command: %s", command.toLocal8Bit().data());
    qWarning("With error: %s", error.toLocal8Bit().data());
    return false;
}

bool AppDb::removeRecord(const int id)
{
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM " + m_tableName + " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    if(!query.exec()){
        return queryError("remove", query.lastError().text());
    }
    return true;
}

