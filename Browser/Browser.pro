QT += core gui network widgets widgets-private quick webenginewidgets sql quickcontrols2 quick3d quickwidgets

CONFIG += c++11 # ordered
CONFIG -= debug_and_release

Api.depends = App Net
Net.depends = App

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    browser.cpp \
    browserpaths.cpp \
    browsersettings.cpp \
    completerheader.cpp \
    completermodel.cpp \
    completerrow.cpp \
    downloaditemproxy.cpp \
    htmlview.cpp \
    main.cpp \
    mainwindow.cpp \
    navigationbar.cpp \
    qbcommon.cpp \
    qmldevtools.cpp \
    qmlview.cpp \
    searchengine.cpp \
    tabview.cpp \
    tabwidget.cpp \
    urllineedit.cpp \
    webpopupwindow.cpp

HEADERS += \
    aboutdialog.h \
    baseview.h \
    browser.h \
    browserpaths.h \
    browsersettings.h \
    completerheader.h \
    completermodel.h \
    completerrow.h \
    downloaditemproxy.h \
    htmlview.h \
    mainwindow.h \
    navigationbar.h \
    qbcommon.h \
    qmldevtools.h \
    qmlview.h \
    searchengine.h \
    tabview.h \
    tabwidget.h \
    urllineedit.h \
    webpopupwindow.h

# PWD expands to directory where project_a.pri placed.
# INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/App
INCLUDEPATH += $$PWD/Net
INCLUDEPATH += $$PWD/Api
DEPENDPATH += $$PWD/App
DEPENDPATH += $$PWD/Net
DEPENDPATH += $$PWD/Api
INCLUDEPATH += $$PWD/../3rdparty/libgit2/include
DEPENDPATH += $$PWD/../3rdparty/libgit2/include


include(../App/App.pri)
include(../Net/Net.pri)
include(../Api/Api.pri)

TRANSLATIONS += \
    q1_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$OUT_PWD/../App/ -lApp
LIBS += -L$$OUT_PWD/../Net/ -lNet
LIBS += -L$$OUT_PWD/../Api/ -lApi
unix {
    target.path = /usr/lib
    LIBS += -L$$PWD/../3rdparty/libgit2/build -lgit2
}

win32 {
    PWD_WIN = $${PWD}
    OUT_WIN = $${OUT_PWD}
    PWD_WIN ~= s,/,\\,g
    OUT_WIN ~= s,/,\\,g
    copyfiles.commands = $$quote(cmd /c xcopy /S /I /Y $${PWD_WIN}\\..\\3rdparty\\libgit2\\build\\Release\git2.dll $${OUT_WIN} && xcopy /S /I /Y $${PWD_WIN}\\styles.qss $${OUT_WIN} && xcopy /S /I /Y $${PWD_WIN}\\icons $${OUT_WIN}\\icons && xcopy /S /I /Y $${PWD_WIN}\\..\\Api\\tools $${OUT_WIN}\\tools)
}
linux {
    copyfiles.commands = cp $$PWD/styles.qss $$OUT_PWD/../ && cp -r $$PWD/icons $$OUT_PWD/../ && cp -r $$PWD/../Api/tools $$OUT_PWD/../
}
first.depends = $(first) copyfiles
export(first.depends)
export(copyfiles.commands)
QMAKE_EXTRA_TARGETS += first copyfiles
