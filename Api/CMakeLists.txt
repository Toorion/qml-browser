cmake_minimum_required(VERSION 3.16)

# Api library
qt_add_library(Api SHARED
    api.cpp
    apicommon.cpp
    apiweb.cpp
    bookmarkdb.cpp
    bookmarkitem.cpp
    bookmarkitemmodel.cpp
    bookmarklinkitem.cpp
    bookmarklinkmodel.cpp
    document.cpp
    downloadbutton.cpp
    downloaddb.cpp
    downloaditemmodel.cpp
    downloadmanagerwidget.cpp
    dynamicobject.cpp
    fetchapi.cpp
    historydb.cpp
    historyitem.cpp
    historyitemmodel.cpp
    location.cpp
    locationurl.cpp
    log.cpp
    qi.cpp
    qmlvalue.cpp
    tools.cpp
    window.cpp
)

set(Api_headers
    Api_global.h
    accessrights.h
    api.h
    apicommon.h
    apiweb.h
    bookmarkdb.h
    bookmarkitem.h
    bookmarkitemmodel.h
    bookmarklinkitem.h
    bookmarklinkmodel.h
    document.h
    downloadbutton.h
    downloaddb.h
    downloaditemmodel.h
    downloadmanagerwidget.h
    dynamicobject.h
    fetchapi.h
    historydb.h
    historyitem.h
    historyitemmodel.h
    location.h
    locationurl.h
    log.h
    qi.h
    qmlvalue.h
    tools.h
    window.h
)

# Qt dependencies
target_link_libraries(Api PUBLIC
    Qt::Qml
    Qt::Widgets
    Qt::Sql
    App
    Net
)

# Private Qt modules - widgets-private
if(Qt6Core_DIR)
    # For Qt6, private headers are in <qt_install>/include/QtCore/<version>/QtCore
    # Get Qt version from package
    get_filename_component(QT_VERSION_FILE "${Qt6Core_DIR}/Qt6CoreConfigVersionImpl.cmake" ABSOLUTE)
    file(READ "${QT_VERSION_FILE}" QT_VERSION_CONTENT)
    string(REGEX MATCH "set\\(PACKAGE_VERSION \"([0-9.]+)\"" _ ${QT_VERSION_CONTENT})
    set(QT_VERSION ${CMAKE_MATCH_1})

    # Qt installation prefix is 3 levels up from Qt6Core_DIR
    get_filename_component(QT_INSTALL_PREFIX "${Qt6Core_DIR}" DIRECTORY)
    get_filename_component(QT_INSTALL_PREFIX "${QT_INSTALL_PREFIX}" DIRECTORY)
    get_filename_component(QT_INSTALL_PREFIX "${QT_INSTALL_PREFIX}" DIRECTORY)

    # Private headers use includes like <QtCore/private/qglobal_p.h>
    # So the include path should be the version directory (parent of QtCore)
    set(QT_VERSION_INCLUDE_DIR "${QT_INSTALL_PREFIX}/include/QtCore/${QT_VERSION}")
    # But the project code uses #include <private/qobject_p.h> which needs QtCore dir
    set(QT_CORE_PRIVATE_INCLUDE_DIR "${QT_VERSION_INCLUDE_DIR}/QtCore")

    if(EXISTS "${QT_CORE_PRIVATE_INCLUDE_DIR}/private/qwidget_p.h")
        target_include_directories(Api SYSTEM PRIVATE ${QT_CORE_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_VERSION_INCLUDE_DIR}/QtWidgets/private/qwidget_p.h")
        target_include_directories(Api SYSTEM PRIVATE ${QT_VERSION_INCLUDE_DIR})
    endif()

    # Also add QtNetwork private headers
    set(QT_NETWORK_VERSION_INCLUDE_DIR "${QT_INSTALL_PREFIX}/include/QtNetwork/${QT_VERSION}")
    set(QT_NETWORK_PRIVATE_INCLUDE_DIR "${QT_NETWORK_VERSION_INCLUDE_DIR}/QtNetwork")

    if(EXISTS "${QT_NETWORK_PRIVATE_INCLUDE_DIR}/private/qnetworkaccessmanager_p.h")
        target_include_directories(Api SYSTEM PRIVATE ${QT_NETWORK_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_NETWORK_VERSION_INCLUDE_DIR}/QtNetwork/private/qnetworkaccessmanager_p.h")
        target_include_directories(Api SYSTEM PRIVATE ${QT_NETWORK_VERSION_INCLUDE_DIR})
    endif()
endif()

# Include directories
target_include_directories(Api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Net>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../App>
    $<INSTALL_INTERFACE:include/Api>
)

# Compile definitions
target_compile_definitions(Api PUBLIC API_LIBRARY)

# Resources (QML tools)
# Note: qmake DISTFILES - we'll handle these separately for deployment

# Install
install(TARGETS Api
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${Api_headers} DESTINATION include/Api)

# Install QML tools
install(DIRECTORY tools/
    DESTINATION share/QmlBrowser/Api/tools
    FILES_MATCHING PATTERN "*.qml"
)