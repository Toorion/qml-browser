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

#include "browser.h"
#include "mainwindow.h"
#include <QWebEngineSettings>
#include <QtQuick3D/qquick3d.h>
#include "location.h"
#include "mainapplication.h"
#include "downloaditem.h"
#include "downloaditemmodel.h"
#include "browsersettings.h"
#include "browserpaths.h"
#include "historyitem.h"
#include "historyitemmodel.h"
#include "bookmarkitem.h"
#include "bookmarkitemmodel.h"
#include "qi.h"
#include "dynamicobject.h"
#include <QDir>
#include <QTimer>
#include <QQuickStyle>
#include <QQmlPropertyMap>
#include "urlhelper.h"
#include "bookmarklinkmodel.h"
#include <QCommandLineParser>



void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (msg.startsWith(QLatin1String("QSslSocket: cannot resolve SSL")))
        return;
    if (msg.startsWith(QLatin1String("Remote debugging server started successfully.")))
        return;

    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
       case QtDebugMsg:
           fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
           break;
       case QtInfoMsg:
           fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
           break;
       case QtWarningMsg:
           fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
           break;
       case QtCriticalMsg:
           fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
           break;
       case QtFatalMsg:
           fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
           break;
    }
    fflush(stderr); // For Win?
}



int main(int argc, char *argv[])
{
    qInstallMessageHandler(&messageHandler);

    // For linux required
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox") ;

    MainApplication app(argc, argv);
    QCoreApplication::setApplicationName("QbqBrowser");
    QCoreApplication::setOrganizationName("QbqSoft");

    QCommandLineParser parser;
    parser.addPositionalArgument("url", "Starting url");
    QCommandLineOption dataPathOption("d", "Browser data directory path", "path");
    parser.addOption(dataPathOption);
    parser.process(app);
    const QStringList args = parser.positionalArguments();
    QString dataPath = parser.value(dataPathOption);

    AppPaths::init(dataPath);

    // Browser icons resource path
    QDir::addSearchPath("icons", BrowserPaths::iconsPath().toLocalFile());
    // Browser styles load
    QFile file(BrowserPaths::resolved("styles.qss").toLocalFile());
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);

    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);

    // Settings
    BrowserSettings *settings = BrowserSettings::instance(BrowserPaths::currentProfilePath() + QLatin1String("/settings.ini"));
    Qi::settings = settings;
    settings->initDefaultUserAgent(QWebEngineProfile::defaultProfile()->httpUserAgent());
    QQuickStyle::setFallbackStyle("Basic");
    QQuickStyle::setStyle(settings->appStyle());

    MainWindow *window = mainBrowser.createWindow();
    window->navigationBar->urlLineEdit->setFocus();

    QMetaType::registerConverter(&Location::toString);

    qRegisterMetaType<DownloadItem*>();
    qRegisterMetaType<DownloadItemModel*>();
    qRegisterMetaType<HistoryItem*>();
    qRegisterMetaType<HistoryItemModel*>();
    qRegisterMetaType<BookmarkItem*>();
    qRegisterMetaType<BookmarkItemModel*>();
    qRegisterMetaType<BookmarkLinkModel*>();
    qRegisterMetaType<DynamicObject*>();
    qRegisterMetaType<QQmlPropertyMap*>();

    QString rawUrl(INTERNAL_URL_SCHEME + QLatin1String("://blank"));
    if (args.size() > 0) {
        rawUrl = args.at(0);
    }
    window->tabWidget->currentView()->loadUrl(rawUrl);

    return app.exec();
}
