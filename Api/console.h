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
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "Api_global.h"

class API_EXPORT Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = nullptr);

    Q_INVOKABLE void log(const QString message);

    Q_INVOKABLE void error(const QString message);

    void clear();

    enum lineType {
        L_ERROR,
        L_LOG
    };
    Q_ENUM(lineType);

    struct logLine {
        QString text;
        lineType type;
    };


    QList<logLine> logRecords();

signals:

    void append(Console::logLine *line);
    void cleared();


private:

    void write(const QString message, const lineType type);

    QList<logLine> logList;

};

#endif // CONSOLE_H
