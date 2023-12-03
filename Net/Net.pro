QT -= gui
QT += core network core-private network-private widgets qml concurrent

TEMPLATE = lib
DEFINES += NET_LIBRARY

CONFIG += c++11
CONFIG -= debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dappinstaller.cpp \
    downloaditem.cpp \
    httpmanager.cpp \
    internalnetworkreply.cpp \
    net.cpp \
    networkaccessmanagerfactory.cpp \
    networkdiskcache.cpp \
    urlhelper.cpp

HEADERS += \
    Net_global.h \
    dappinstaller.h \
    downloaditem.h \
    downloaditem_p.h \
    httpmanager.h \
    internalnetworkreply.h \
    net.h \
    networkaccessmanagerfactory.h \
    networkdiskcache.h \
    networkdiskcache_p.h \
    semver.h \
    urlhelper.h

INCLUDEPATH += $$PWD/App
DEPENDPATH += $$PWD/App
INCLUDEPATH += $$PWD/../3rdparty/libgit2/include
DEPENDPATH += $$PWD/../3rdparty/libgit2/include

include(../App/App.pri)

LIBS += -L$$OUT_PWD/../App/ -lApp
LIBS += -L$$PWD/../3rdparty/libgit2/build -lgit2
# LIBS += -L$$OUT_PWD/../libgit2 -lgit2

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target


# QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/../libgit2\''
# QMAKE_RPATHDIR += $ORIGIN/../mylibs
