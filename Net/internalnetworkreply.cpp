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
#include "internalnetworkreply.h"
#include <QTimer>
#include <QByteArray>
#include <QUrl>

struct InternalNetworkReplyPrivate
{
    QByteArray content;
    qint64 offset;
};

InternalNetworkReply::InternalNetworkReply(QObject *parent)
    : QNetworkReply{parent}
{
    d = new InternalNetworkReplyPrivate();
}

InternalNetworkReply::~InternalNetworkReply()
{
    delete d;
}

void InternalNetworkReply::setHttpStatusCode(int code, const QByteArray &statusText)
{
    setAttribute( QNetworkRequest::HttpStatusCodeAttribute, code );
    if ( statusText.isNull() )
        return;

    setAttribute( QNetworkRequest::HttpReasonPhraseAttribute, statusText );
}

void InternalNetworkReply::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    QNetworkReply::setHeader( header, value );
}

void InternalNetworkReply::setContentType(const QByteArray &contentType)
{
    setHeader(QNetworkRequest::ContentTypeHeader, contentType);
}

void InternalNetworkReply::setContent(const QString &content)
{
    setContent(content.toUtf8());
}

void InternalNetworkReply::setContent(const QByteArray &content)
{
    d->offset = 0;
    d->content = content;

    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));

    QTimer::singleShot( 0, this, SIGNAL(readyRead()) );
    QTimer::singleShot( 0, this, SIGNAL(finished()) );
}

void InternalNetworkReply::setUrl(const QUrl &url)
{
    QNetworkReply::setUrl(url);
}


void InternalNetworkReply::abort()
{

}

qint64 InternalNetworkReply::bytesAvailable() const
{
    return d->content.size() - d->offset + QIODevice::bytesAvailable();
}

bool InternalNetworkReply::isSequential() const
{
    return true;
}

qint64 InternalNetworkReply::readData(char *data, qint64 maxSize)
{
    if (d->offset >= d->content.size())
        return -1;

    qint64 number = qMin(maxSize, d->content.size() - d->offset);
    memcpy(data, d->content.constData() + d->offset, number);
    d->offset += number;

    return number;
}

