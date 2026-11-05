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
#ifndef FETCHAPI_H
#define FETCHAPI_H

#include <QObject>
#include <QJSValue>
#include <QUrl>
#include <QVariantMap>
#include <QJSEngine>
#include "Api_global.h"

class NetworkAccessManagerFactory;

class API_EXPORT FetchApi : public QObject
{
    Q_OBJECT
public:
    explicit FetchApi(NetworkAccessManagerFactory *factory, QJSEngine *engine, QObject *parent = nullptr);

    Q_INVOKABLE QJSValue fetch(const QUrl &url, const QVariantMap &options = QVariantMap());

private:
    NetworkAccessManagerFactory *m_factory;
    QJSEngine *m_engine;
};

#endif // FETCHAPI_H