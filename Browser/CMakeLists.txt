cmake_minimum_required(VERSION 3.16)

# Browser executable
qt_add_executable(Browser
    aboutdialog.cpp
    browser.cpp
    browserpaths.cpp
    browsersettings.cpp
    completerheader.cpp
    completermodel.cpp
    completerrow.cpp
    downloaditemproxy.cpp
    htmlview.cpp
    main.cpp
    mainwindow.cpp
    navigationbar.cpp
    qbcommon.cpp
    qmldevtools.cpp
    qmlview.cpp
    searchengine.cpp
    tabview.cpp
    tabwidget.cpp
    urllineedit.cpp
    webpopupwindow.cpp
)

# Translation (disabled for now - qt_add_translation has issues with source paths)
# qt_add_translation(Browser_TRANSLATIONS q1_ru_RU.ts)
# target_sources(Browser PRIVATE ${Browser_TRANSLATIONS})

# Qt dependencies
target_link_libraries(Browser PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
    Qt::Quick
    Qt::WebEngineWidgets
    Qt::Sql
    Qt::QuickControls2
    Qt::Quick3D
    Qt::QuickWidgets
    App
    Net
    Api
    libgit2package
    ${ZLIB_LIBRARIES}
)

# Private Qt modules
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
        target_include_directories(Browser SYSTEM PRIVATE ${QT_CORE_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_VERSION_INCLUDE_DIR}/QtWidgets/private/qwidget_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_VERSION_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_CORE_PRIVATE_INCLUDE_DIR}/private/qcoreapplication_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_CORE_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_VERSION_INCLUDE_DIR}/QtCore/private/qcoreapplication_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_VERSION_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_CORE_PRIVATE_INCLUDE_DIR}/private/qnetworkaccessmanager_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_CORE_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_VERSION_INCLUDE_DIR}/QtNetwork/private/qnetworkaccessmanager_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_VERSION_INCLUDE_DIR})
    endif()

    # Also add QtNetwork private headers
    set(QT_NETWORK_VERSION_INCLUDE_DIR "${QT_INSTALL_PREFIX}/include/QtNetwork/${QT_VERSION}")
    set(QT_NETWORK_PRIVATE_INCLUDE_DIR "${QT_NETWORK_VERSION_INCLUDE_DIR}/QtNetwork")

    if(EXISTS "${QT_NETWORK_PRIVATE_INCLUDE_DIR}/private/qnetworkaccessmanager_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_NETWORK_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_NETWORK_VERSION_INCLUDE_DIR}/QtNetwork/private/qnetworkaccessmanager_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_NETWORK_VERSION_INCLUDE_DIR})
    endif()

    # Also add QtWidgets private headers for qwindowcontainer_p.h
    set(QT_WIDGETS_VERSION_INCLUDE_DIR "${QT_INSTALL_PREFIX}/include/QtWidgets/${QT_VERSION}")
    set(QT_WIDGETS_PRIVATE_INCLUDE_DIR "${QT_WIDGETS_VERSION_INCLUDE_DIR}/QtWidgets")

    if(EXISTS "${QT_WIDGETS_PRIVATE_INCLUDE_DIR}/private/qwindowcontainer_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_WIDGETS_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_WIDGETS_VERSION_INCLUDE_DIR}/QtWidgets/private/qwindowcontainer_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_WIDGETS_VERSION_INCLUDE_DIR})
    endif()

    # Also add QtGui private headers for qtguiglobal_p.h (needed by qtwidgetsglobal_p.h)
    # and qpa headers (needed by qwidget_p.h)
    set(QT_GUI_VERSION_INCLUDE_DIR "${QT_INSTALL_PREFIX}/include/QtGui/${QT_VERSION}")
    set(QT_GUI_PRIVATE_INCLUDE_DIR "${QT_GUI_VERSION_INCLUDE_DIR}/QtGui")

    if(EXISTS "${QT_GUI_PRIVATE_INCLUDE_DIR}/private/qtguiglobal_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_GUI_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_GUI_VERSION_INCLUDE_DIR}/QtGui/private/qtguiglobal_p.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_GUI_VERSION_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_GUI_PRIVATE_INCLUDE_DIR}/qpa/qplatformbackingstore.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_GUI_PRIVATE_INCLUDE_DIR})
    endif()
    if(EXISTS "${QT_GUI_VERSION_INCLUDE_DIR}/qpa/qplatformbackingstore.h")
        target_include_directories(Browser SYSTEM PRIVATE ${QT_GUI_VERSION_INCLUDE_DIR})
    endif()
endif()

# Include directories
target_include_directories(Browser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/App>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Net>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Api>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3rdparty/libgit2/include>
)

# Link directories for libgit2
if(WIN32)
    target_link_directories(Browser PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build/Release)
else()
    target_link_directories(Browser PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build)
    set_target_properties(Browser PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build"
        INSTALL_RPATH "\$ORIGIN/../lib"
    )
endif()

# Copy assets (styles.qss, icons/, Api/tools/) to build directory
# This replaces the qmake copyfiles target
add_custom_command(TARGET Browser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/styles.qss
        ${CMAKE_CURRENT_BINARY_DIR}/../styles.qss
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/icons
        ${CMAKE_CURRENT_BINARY_DIR}/../icons
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../Api/tools
        ${CMAKE_CURRENT_BINARY_DIR}/../tools
    COMMENT "Copying browser assets (styles, icons, tools)"
)

# Copy libgit2 DLL on Windows
if(WIN32)
    add_custom_command(TARGET Browser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/3rdparty/libgit2/build/Release/git2.dll
            ${CMAKE_CURRENT_BINARY_DIR}/git2.dll
        COMMENT "Copying libgit2 DLL"
    )
endif()

# Install
install(TARGETS Browser
    RUNTIME DESTINATION bin
)

# Install assets
install(FILES styles.qss DESTINATION share/QmlBrowser)
install(DIRECTORY icons/ DESTINATION share/QmlBrowser/icons)
install(DIRECTORY ../Api/tools/ DESTINATION share/QmlBrowser/tools
    FILES_MATCHING PATTERN "*.qml"
)

# Install translations
install(FILES q1_ru_RU.ts DESTINATION share/QmlBrowser/translations)