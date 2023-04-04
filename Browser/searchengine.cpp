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
#include "searchengine.h"
#include <QUrlQuery>
#include "browsersettings.h"

#define ENGINE_GOOGLE QLatin1String("google.com")
#define ENGINE_DUCKDUCKGO QLatin1String("duckduckgo.com")
#define ENGINE_YAHOO QLatin1String("yahoo.com")

QStringList SearchEngine::engineList = {
    ENGINE_GOOGLE,
    ENGINE_DUCKDUCKGO,
    ENGINE_YAHOO
};

SearchEngine::SearchEngine(QObject *parent)
    : QObject{parent}
{

}

QUrl SearchEngine::searchUrl(QString text)
{
    QString engineName = browserSettings->searchEngine();
    QUrl url;
    QUrlQuery query;
    if(engineName == ENGINE_DUCKDUCKGO) {
        url.setUrl("https://duckduckgo.com/");
        query.addQueryItem(QLatin1String("q"), text);
    } else if( engineName == ENGINE_YAHOO) {
        url.setUrl("https://search.yahoo.com/search");
        query.addQueryItem(QLatin1String("p"), text);
    } else {
        url.setUrl("https://www.google.com/search");
        query.addQueryItem(QLatin1String("q"), text);
    }
    url.setQuery(query);
    return url;
}
