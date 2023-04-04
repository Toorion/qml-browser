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
#include "qmldevtools.h"
#include <QVBoxLayout>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QBrush>
#include <QColor>

QmlDevTools::QmlDevTools(Console *console, QWidget *parent) : QWidget(parent),
    console(console)
{
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    logModel = new QStandardItemModel;
    QList <Console::logLine> records = console->logRecords();
    for (int i = 0; i < records.count(); ++i) {
        writeLogLine(&records.at(i));
    }

    logView = new QListView(this);
    logView->setModel(logModel);
    logView->scrollToBottom();

    autoScrollToBottom = true;

    connect(console, &Console::append, this, &QmlDevTools::writeLogLine);

    layout->addWidget(logView);
}


void QmlDevTools::writeLogLine(const Console::logLine *line)
{
    QList<QStandardItem *> listItem;
    QStandardItem *item = new QStandardItem();
    item->setText(line->text);

    if(Console::lineType::L_LOG == line->type) {
        item->setForeground(QBrush(QColor(140,140,140)));
    } else if(Console::lineType::L_ERROR == line->type) {
        item->setForeground(QBrush(QColor(255,0,0)));
    }

    listItem << item;
    logModel->appendRow(listItem);

    if(autoScrollToBottom) {
        logView->scrollToBottom();
    }
}
