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
#ifndef INTERNALNETWORKREPLY_H
#define INTERNALNETWORKREPLY_H

#include <QNetworkReply>

class InternalNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    explicit InternalNetworkReply(QObject *parent = nullptr);
    ~InternalNetworkReply();

        void setHttpStatusCode( int code, const QByteArray &statusText = QByteArray() );
        void setHeader( QNetworkRequest::KnownHeaders header, const QVariant &value );
        void setContentType( const QByteArray &contentType );

        void setContent( const QString &content );
        void setContent( const QByteArray &content );
        void setUrl(const QUrl &url);

        void abort();
        qint64 bytesAvailable() const;
        bool isSequential() const;

    protected:
        qint64 readData(char *data, qint64 maxSize);

    private:
        struct InternalNetworkReplyPrivate *d;
};

#endif // INTERNALNETWORKREPLY_H
