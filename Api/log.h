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
#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "Api_global.h"

class API_EXPORT Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

    Q_INVOKABLE void debug(const QString message);

    Q_INVOKABLE void error(const QString message);

    void clear();

    enum lineType {
        L_DEBUG,
        L_ERROR
    };
    Q_ENUM(lineType);

    struct logLine {
        QString text;
        lineType type;
    };


    QList<logLine> logRecords();

signals:

    void append(Log::logLine *line);
    void cleared();


private:

    void write(const QString message, const lineType type);

    QList<logLine> logList;

};

#endif // LOG_H
