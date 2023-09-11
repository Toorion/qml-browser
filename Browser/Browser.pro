QT += quick3d
QT += core gui network widgets widgets-private quick webenginewidgets sql quickcontrols2

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
    webpopupwindow.cpp

HEADERS += \
    aboutdialog.h \
    baseview.h \
    browser.h \
    browserpaths.h \
    browsersettings.h \
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
    webpopupwindow.h

# PWD expands to directory where project_a.pri placed.
# INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/App
INCLUDEPATH += $$PWD/Net
INCLUDEPATH += $$PWD/Api
DEPENDPATH += $$PWD/App
DEPENDPATH += $$PWD/Net
DEPENDPATH += $$PWD/Api

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

# https://forum.qt.io/topic/119599/qtwebengine-link-errors/5
# For QT6 not neccessary
# QMAKE_LFLAGS += -fuse-ld=gold
