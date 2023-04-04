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
#ifndef HISTORYDB_H
#define HISTORYDB_H

#include "historyitem.h"
#include "appdb.h"
#include "Api_global.h"

class API_EXPORT HistoryDb : public AppDb
{
public:
    HistoryDb();

    bool create() override;

    int static insert(const HistoryItem &item);

    bool static update(const HistoryItem &item);

    bool static remove(const int id);

    QList<HistoryItem*> static list();

};

#endif // HISTORYDB_H
