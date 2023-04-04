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
#ifndef APPDB_H
#define APPDB_H

#include <QObject>
#include <QSqlDatabase>
#include "App_global.h"

class APP_EXPORT AppDb : public QObject
{
    Q_OBJECT
public:
    explicit AppDb();

    const QString dbFullName();
    bool connect();
    bool open();
    void close();
    virtual bool create() = 0;

    bool queryError(const QString operation, const QString query);
    bool removeRecord(const int id);

    QLatin1String dbName;


signals:


protected:

    QLatin1String m_dbName;

    QLatin1String m_tableName;

    QSqlDatabase m_db;

};

#endif // APPDB_H
