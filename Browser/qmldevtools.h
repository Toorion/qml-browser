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
#ifndef QMLDEVTOOLS_H
#define QMLDEVTOOLS_H

#include <QWidget>
#include <QVBoxLayout>
#include <log.h>
#include <QListView>
#include <QStandardItemModel>

class QmlDevTools : public QWidget
{
    Q_OBJECT
public:
    explicit QmlDevTools(Log *log, QWidget *parent = nullptr);

public Q_SLOTS:
    void writeLogLine(const Log::logLine *line);

    void clearLog();

private:
    QVBoxLayout *layout;

    Log *log;

    QListView *logView;

    QStandardItemModel *logModel;

    bool autoScrollToBottom = false;

};

#endif // QMLDEVTOOLS_H
