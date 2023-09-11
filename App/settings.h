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
#ifndef SETTINGS_H
#define SETTINGS_H

#include "App_global.h"
#include <QSettings>

class APP_EXPORT Settings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(QString appUserAgent READ appUserAgent WRITE setAppUserAgent NOTIFY appUserAgentChanged)
    Q_PROPERTY(QString appCacheMaxSize READ appCacheMaxSize WRITE setAppCacheMaxSize NOTIFY appCacheMaxSizeChanged)
    Q_PROPERTY(QString appStyle READ appStyle WRITE setAppStyle NOTIFY appStyleChanged)
public:
    explicit Settings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr);

    void setAppUserAgent(QString value);
    QString appUserAgent();

    void setAppCacheMaxSize(QString value);
    QString appCacheMaxSize();

    void setAppStyle(QString value);
    QString appStyle();

    void initDefaultUserAgent(QString userAgent);
    QString defaultUserAgent();

signals:
    void appUserAgentChanged(QString userAgent);
    void appStyleChanged(QString style);
    void appCacheMaxSizeChanged(qint8 appCacheMaxSize);
private:
    QString m_defaultUserAgent;
};

#endif // SETTINGS_H
