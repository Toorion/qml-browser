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
#include "fetchapi.h"
#include "networkaccessmanagerfactory.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJSEngine>
#include <QJSValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QUrlQuery>
#include <QTimer>

FetchApi::FetchApi(NetworkAccessManagerFactory *factory, QJSEngine *engine, QObject *parent)
    : QObject(parent), m_factory(factory), m_engine(engine)
{
}

QJSValue FetchApi::fetch(const QUrl &url, const QVariantMap &options)
{
    QJSEngine *engine = m_engine;
    if (!engine) {
        qWarning() << "FetchApi: No QJSEngine available";
        return QJSValue();
    }

    // Create Promise using a standard pattern: create resolve/reject variables in JS
    // and return the promise
    QJSValue promise = engine->evaluate(
        "(function() { "
        "    var resolveRef = null; "
        "    var rejectRef = null; "
        "    var promise = new Promise(function(resolve, reject) { "
        "        resolveRef = resolve; "
        "        rejectRef = reject; "
        "    }); "
        "    return { promise: promise, resolve: resolveRef, reject: rejectRef }; "
        "})()"
    );
    
    if (!promise.isObject()) {
        qWarning() << "FetchApi: Failed to create Promise";
        return QJSValue();
    }

    QJSValue resolve = promise.property("resolve");
    QJSValue reject = promise.property("reject");
    QJSValue promiseResult = promise.property("promise");

    if (!resolve.isCallable() || !reject.isCallable()) {
        qWarning() << "FetchApi: Failed to create Promise with resolve/reject";
        return QJSValue();
    }

    QNetworkRequest request(url);

    // HTTP method
    QString method = options.value("method", "GET").toString().toUpper();
    QByteArray bodyData;
    if (options.contains("body")) {
        QVariant body = options["body"];
        if (body.typeId() == QMetaType::QString) {
            bodyData = body.toString().toUtf8();
        } else if (body.typeId() == QMetaType::QByteArray) {
            bodyData = body.toByteArray();
        } else if (body.typeId() == QMetaType::QVariantMap || body.typeId() == QMetaType::QJsonObject) {
            QJsonDocument doc(QJsonObject::fromVariantMap(body.toMap()));
            bodyData = doc.toJson(QJsonDocument::Compact);
            if (!request.hasRawHeader("Content-Type")) {
                request.setRawHeader("Content-Type", "application/json");
            }
        }
    }

    // Headers
    if (options.contains("headers")) {
        QVariantMap headers = options["headers"].toMap();
        for (auto it = headers.begin(); it != headers.end(); ++it) {
            request.setRawHeader(it.key().toUtf8(), it.value().toByteArray());
        }
    }

    // Credentials mode
    if (options.contains("credentials")) {
        QString credentials = options["credentials"].toString();
        if (credentials == "include") {
            // Cookies are handled automatically by QNetworkAccessManager
        } else if (credentials == "omit") {
            request.setAttribute(QNetworkRequest::CookieSaveControlAttribute, false);
        }
    }

    // Cache mode
    if (options.contains("cache")) {
        QString cache = options["cache"].toString();
        if (cache == "no-store") {
            request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        } else if (cache == "no-cache") {
            request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork);
        }
    }

    // Redirect mode - Qt 6 doesn't have FollowRedirectsAttribute
    if (options.contains("redirect")) {
        QString redirect = options["redirect"].toString();
        if (redirect == "error" || redirect == "manual") {
            // Qt 6 handles redirects automatically, can't easily disable
            // Could implement custom redirect handling if needed
        }
    }

    QNetworkAccessManager *nam = m_factory->create(this);
    QNetworkReply *reply = nullptr;

    if (method == "GET") {
        reply = nam->get(request);
    } else if (method == "POST") {
        reply = nam->post(request, bodyData);
    } else if (method == "PUT") {
        reply = nam->put(request, bodyData);
    } else if (method == "DELETE") {
        reply = nam->deleteResource(request);
    } else if (method == "PATCH") {
        reply = nam->sendCustomRequest(request, "PATCH", bodyData);
    } else if (method == "HEAD") {
        reply = nam->head(request);
    } else if (method == "OPTIONS") {
        reply = nam->sendCustomRequest(request, "OPTIONS", bodyData);
    } else {
        reply = nam->sendCustomRequest(request, method.toUtf8(), bodyData);
    }

    connect(reply, &QNetworkReply::finished, this, [reply, resolve, reject, engine]() {
        QJSValue response = engine->newObject();
        response.setProperty("ok", reply->error() == QNetworkReply::NoError);
        response.setProperty("status", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        response.setProperty("statusText", reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
        response.setProperty("url", reply->url().toString());
        response.setProperty("type", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 0 ? "error" : "basic");

        // Headers
        QJSValue headersObj = engine->newObject();
        QList<QByteArray> headerList = reply->rawHeaderList();
        for (const QByteArray &headerName : headerList) {
            headersObj.setProperty(QString::fromUtf8(headerName), QString::fromUtf8(reply->rawHeader(headerName)));
        }
        response.setProperty("headers", headersObj);

        QByteArray data = reply->readAll();

        // text() method
        QString dataStr = QString::fromUtf8(data).replace("'", "\\'").replace("\n", "\\n").replace("\r", "\\r");
        QJSValue textFn = engine->evaluate("(function() { return '" + dataStr + "'; })");
        response.setProperty("text", textFn);

        // json() method
        QJSValue jsonFn = engine->evaluate("(function() { "
            "try { return JSON.parse('" + dataStr + "'); } "
            "catch(e) { throw new Error('JSON parse error: ' + e.message); } "
            "})");
        response.setProperty("json", jsonFn);

        // arrayBuffer() method
        QJSValue arrayBufferFn = engine->evaluate("(function() { "
            "var arr = new Uint8Array(" + QString::number(data.size()) + "); "
            "return arr; "
            "})");
        response.setProperty("arrayBuffer", arrayBufferFn);

        // blob() method
        QString contentType = QString::fromUtf8(reply->header(QNetworkRequest::ContentTypeHeader).toByteArray()).replace("'", "\\'");
        QJSValue blobFn = engine->evaluate("(function() { "
            "return { size: " + QString::number(data.size()) + ", "
            "type: '" + contentType + "', "
            "arrayBuffer: function() { return new Uint8Array(" + QString::number(data.size()) + "); } "
            "}; "
            "})");
        response.setProperty("blob", blobFn);

        // clone() method
        QJSValue cloneFn = engine->evaluate("(function() { "
            "var cloned = Object.create(Object.getPrototypeOf(this)); "
            "for (var key in this) { if (this.hasOwnProperty(key)) cloned[key] = this[key]; } "
            "return cloned; "
            "})");
        response.setProperty("clone", cloneFn);

        if (reply->error() == QNetworkReply::NoError) {
            resolve.call({response});
        } else {
            QJSValue error = engine->evaluate("new Error('" + reply->errorString().replace("'", "\\'") + "')");
            error.setProperty("response", response);
            reject.call({error});
        }
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::errorOccurred, this, [reply, reject, engine](QNetworkReply::NetworkError error) {
        if (error != QNetworkReply::NoError) {
            QJSValue err = engine->evaluate("new Error('" + reply->errorString().replace("'", "\\'") + "')");
            err.setProperty("code", static_cast<int>(error));
            reject.call({err});
        }
    });

    // Timeout handling
    if (options.contains("timeout")) {
        int timeout = options["timeout"].toInt();
        if (timeout > 0) {
            QTimer::singleShot(timeout, this, [reply, reject, engine]() {
                if (reply && reply->isRunning()) {
                    reply->abort();
                    QJSValue err = engine->evaluate("new Error('Request timeout')");
                    err.setProperty("code", "TIMEOUT");
                    reject.call({err});
                }
            });
        }
    }

    return promiseResult;
}