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
#ifndef APICOMMON_H
#define APICOMMON_H

#include <QObject>
#include "console.h"
#include "document.h"
#include "qi.h"
#include "downloaditem.h"
#include "accessrights.h"
#include "httpmanager.h"
#include "Api_global.h"

class API_EXPORT ApiCommon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Console *console READ console CONSTANT)
    Q_PROPERTY(Qi *qi READ qi CONSTANT)

public:
    explicit ApiCommon(HttpManager *httpManager, QObject *parent = nullptr);

    Console *console() {return m_console;}

    Qi *qi() {return m_qi;}

    Q_INVOKABLE void alert(const QString message, const QString title = "");

    Q_INVOKABLE bool confirm(const QString message, const QString title = "");

    Q_INVOKABLE DownloadItem *download(const QString url);

    Q_INVOKABLE QString preload(const QString url);

    void setAccessRights(AccessRights *accessRights);

    AccessRights *accessRights();

signals:
    void requestDownload(const QString url);

private:
    Console *m_console;

    Qi *m_qi;

    AccessRights *m_accessRights;

    HttpManager *m_httpManager;

};


#endif // APICOMMON_H
