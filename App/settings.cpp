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
#include "settings.h"
#include <QSettings>


#define APP_GROUP QLatin1String("App")
#define APP_DEFAULT_STYLE QLatin1String("Basic")

Settings::Settings(const QString &fileName, Format format, QObject *parent)
    : QSettings(fileName, format, parent)
{

}

void Settings::setAppUserAgent(QString value)
{
    beginGroup(APP_GROUP);
    setValue("UserAgent", value);
    endGroup();
    //emit appUserAgentChanged(value);
}

QString Settings::appUserAgent()
{
    beginGroup(APP_GROUP);
    QString v = value("UserAgent").toString();
    if(v.isEmpty()) {
        v = m_defaultUserAgent;
    }
    endGroup();
    return v;
}

void Settings::setAppCacheMaxSize(QString value)
{
    beginGroup(APP_GROUP);
    setValue("CacheMaxSize", value);
    endGroup();
}

QString Settings::appCacheMaxSize()
{
    beginGroup(APP_GROUP);
    QString v = value("CacheMaxSize").toString();
    if(v.toInt() < 50) {
        v = "50";
    }
    endGroup();
    return v;
}

void Settings::setAppStyle(QString value)
{
    beginGroup(APP_GROUP);
    setValue("Style", value);
    endGroup();
}

QString Settings::appStyle()
{
    beginGroup(APP_GROUP);
    QString v = value("Style").toString();
    if(v.isEmpty()) {
        v = APP_DEFAULT_STYLE;
    }
    endGroup();
    return v;
}



void Settings::initDefaultUserAgent(QString userAgent)
{
    // Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) QtWebEngine/5.15.2 Chrome/83.0.4103.122 Safari/537.36
    QString browser = userAgent.mid(userAgent.indexOf("QtWebEngine"));
    browser = browser.mid(0, browser.indexOf(" "));
    m_defaultUserAgent = userAgent.replace(browser, "QBQBrowser/1.0.0");
}

QString Settings::defaultUserAgent()
{
    return m_defaultUserAgent;
}

