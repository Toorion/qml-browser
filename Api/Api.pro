QT -= gui
QT += qml widgets widgets-private sql

TEMPLATE = lib
DEFINES += API_LIBRARY

CONFIG += c++11
CONFIG -= debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    api.cpp \
    apicommon.cpp \
    apiweb.cpp \
    console.cpp \
    document.cpp \
    downloadbutton.cpp \
    downloaddb.cpp \
    downloaditemmodel.cpp \
    downloadmanagerwidget.cpp \
    dynamicobject.cpp \
    historydb.cpp \
    historyitem.cpp \
    historyitemmodel.cpp \
    location.cpp \
    locationurl.cpp \
    qi.cpp \
    tools.cpp \
    window.cpp

HEADERS += \
    Api_global.h \
    accessrights.h \
    api.h \
    apicommon.h \
    apiweb.h \
    console.h \
    document.h \
    downloadbutton.h \
    downloaddb.h \
    downloaditemmodel.h \
    downloadmanagerwidget.h \
    dynamicobject.h \
    historydb.h \
    historyitem.h \
    historyitemmodel.h \
    location.h \
    locationurl.h \
    qi.h \
    tools.h \
    window.h

INCLUDEPATH += $$PWD/../Net
INCLUDEPATH += $$PWD/../App

include(../App/App.pri)
include(../Net/Net.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Api.pri \
    tools/downloads/main.qml \
    tools/error/main.qml

LIBS += -L$$OUT_PWD/../App/ -lApp
LIBS += -L$$OUT_PWD/../Net/ -lNet


