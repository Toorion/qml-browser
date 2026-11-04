cmake_minimum_required(VERSION 3.16)

# Net library
qt_add_library(Net SHARED
    dappinstaller.cpp
    downloaditem.cpp
    httpmanager.cpp
    internalnetworkreply.cpp
    net.cpp
    networkaccessmanagerfactory.cpp
    networkdiskcache.cpp
    urlhelper.cpp
)

set(Net_headers
    Net_global.h
    dappinstaller.h
    downloaditem.h
    downloaditem_p.h
    httpmanager.h
    internalnetworkreply.h
    net.h
    networkaccessmanagerfactory.h
    networkdiskcache.h
    networkdiskcache_p.h
    semver.h
    urlhelper.h
)

# Qt dependencies - note: Qt::CorePrivate, Qt::NetworkPrivate, Qt::WidgetsPrivate
# These are not standard Qt6 targets. We'll need to find them manually.
target_link_libraries(Net PUBLIC
    Qt::Core
    Qt::Network
    Qt::Widgets
    Qt::Qml
    Qt::Concurrent
    App
    libgit2package
)

# Include directories
target_include_directories(Net PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../App>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3rdparty/libgit2/include>
    $<INSTALL_INTERFACE:include/Net>
)

# Private Qt modules - need to find include paths
# These are internal Qt headers, we add them as system includes
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

    if(EXISTS "${QT_CORE_PRIVATE_INCLUDE_DIR}/private/qcoreapplication_p.h")
        target_include_directories(Net SYSTEM PRIVATE ${QT_CORE_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_VERSION_INCLUDE_DIR}/QtCore/private/qcoreapplication_p.h")
        target_include_directories(Net SYSTEM PRIVATE ${QT_VERSION_INCLUDE_DIR})
    endif()

    # Also add QtNetwork private headers for qabstractnetworkcache_p.h etc.
    set(QT_NETWORK_VERSION_INCLUDE_DIR "${QT_INSTALL_PREFIX}/include/QtNetwork/${QT_VERSION}")
    set(QT_NETWORK_PRIVATE_INCLUDE_DIR "${QT_NETWORK_VERSION_INCLUDE_DIR}/QtNetwork")

    if(EXISTS "${QT_NETWORK_PRIVATE_INCLUDE_DIR}/private/qabstractnetworkcache_p.h")
        target_include_directories(Net SYSTEM PRIVATE ${QT_NETWORK_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_NETWORK_VERSION_INCLUDE_DIR}/QtNetwork/private/qabstractnetworkcache_p.h")
        target_include_directories(Net SYSTEM PRIVATE ${QT_NETWORK_VERSION_INCLUDE_DIR})
    endif()
endif()

# Compile definitions
target_compile_definitions(Net PUBLIC NET_LIBRARY)

# Link libgit2
if(WIN32)
    # On Windows, libgit2 is in build/Release
    target_link_directories(Net PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build/Release)
else()
    target_link_directories(Net PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build)
    # RPATH for libgit2
    set_target_properties(Net PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build"
        INSTALL_RPATH "\$ORIGIN/../lib"
    )
endif()

# Install
install(TARGETS Net
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${Net_headers} DESTINATION include/Net)