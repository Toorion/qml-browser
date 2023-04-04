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
#include "qi.h"
#include "dynamicobject.h"
#include <QDir>
#include <QTimer>


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


QUrl commandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg : args.mid(1)) {
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    return QUrl();
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(&messageHandler);

    QCoreApplication::setOrganizationName("QbqBrowser");

    MainApplication app(argc, argv);

    //qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --no-sandbox") ;
    // For linux required
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox") ;

    // Browser styles load
    QDir::setSearchPaths("icons", QStringList(BrowserPaths::iconsPath()));
    QFile file(BrowserPaths::rootPath() + "/styles.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);

    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
    // todo - disable this: QWebEngineSettings::LocalStorageEnabled
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);

    // Settings
    Qi::settings = BrowserSettings::instance(BrowserPaths::currentProfilePath() + QLatin1String("/settings.ini"));
    BrowserSettings::instance()->initDefaultUserAgent(QWebEngineProfile::defaultProfile()->httpUserAgent());

    MainWindow *window = mainBrowser.createWindow();
    window->navigationBar->urlLineEdit->setFocus();

    QMetaType::registerConverter(&Location::toString);

    qRegisterMetaType<DownloadItem*>();
    qRegisterMetaType<DownloadItemModel*>();
    qRegisterMetaType<HistoryItem*>();
    qRegisterMetaType<HistoryItemModel*>();
    qRegisterMetaType<DynamicObject*>();


    /*
     * Important! Qt6 Hook - QWebEngine should be initialized before any windowContainer!
     */
    QWebEngineView *tmpView = new QWebEngineView(window);
    tmpView->setContent(QByteArray());
    QTimer::singleShot(1000, [=](){
        tmpView->deleteLater();
    });

    QUrl url = commandLineUrlArgument();
    if(url.isEmpty()) {
        url.setUrl(INTERNAL_URL_SCHEME + QLatin1String("://blank"));
    }
    window->tabWidget->currentView()->loadUrl(url);

    return app.exec();
}
