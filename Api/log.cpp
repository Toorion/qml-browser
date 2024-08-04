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
#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::debug(const QString message)
{
    write(message, lineType::L_DEBUG);
}


void Log::error(const QString message)
{
    write(message, lineType::L_ERROR);
}

void Log::clear()
{
    logList.clear();
    emit cleared();
}

QList<Log::logLine> Log::logRecords()
{
    return logList;
}

void Log::write(const QString message, const Log::lineType type)
{
    logLine line = logLine{message, type};
    logList << line;
    emit append(&line);
}

