QT += widgets sql

TEMPLATE = lib

CONFIG += c++11
DEFINES += APP_LIBRARY
CONFIG -= debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    appdb.cpp \
    apppaths.cpp \
    mainapplication.cpp \
    settings.cpp

HEADERS += \
    App_global.h \
    app.h \
    appdb.h \
    apppaths.h \
    mainapplication.h \
    settings.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
