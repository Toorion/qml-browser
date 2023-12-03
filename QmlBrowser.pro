TEMPLATE = subdirs

# https://wiki.qt.io/SUBDIRS_-_handling_dependencies
SUBDIRS += \
    App \
    Net \
    Api \
    Browser

CONFIG += ordered

# unix:{
#     QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
# }

