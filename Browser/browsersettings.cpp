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
#include "browsersettings.h"
#include "searchengine.h"

#define BROWSER_GROUP QLatin1String("Browser")

BrowserSettings::BrowserSettings(const QString &fileName, Format format, QObject *parent)
    : Settings(fileName, format, parent)
{

}

BrowserSettings *BrowserSettings::instance(const QString &fileName, QObject *parent)
{
    static BrowserSettings instance(fileName, QSettings::IniFormat, parent);
    return &instance;
}

void BrowserSettings::setSearchEngine(QString value)
{
    beginGroup(BROWSER_GROUP);
    setValue("SearchEngine", value);
    endGroup();

}

QString BrowserSettings::searchEngine()
{
    beginGroup(BROWSER_GROUP);
    QString v = value("SearchEngine").toString();
    if(v.isEmpty()) {
        v = QString("google.com");
    }
    endGroup();
    return v;
}

QStringList BrowserSettings::searchEngineList()
{
    return SearchEngine::engineList;
}

