cmake_minimum_required(VERSION 3.16)
project(QmlBrowser VERSION 1.0.0 LANGUAGES CXX)

# Qt6 setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable debug_and_release equivalent
set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Global options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Find Qt6
find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Sql
    Qml
    Quick
    QuickWidgets
    QuickControls2
    Quick3D
    WebEngineWidgets
    WebEngineCore
    Concurrent
    Positioning
    PrintSupport
    WebChannel
    ShaderTools
    OpenGL
    LinguistTools
)

# Find ZLIB
find_package(ZLIB REQUIRED)

# Include libgit2 as subdirectory (built as part of main build)
add_subdirectory(3rdparty/libgit2)

# Subdirectories in dependency order
add_subdirectory(App)
add_subdirectory(Net)
add_subdirectory(Api)
add_subdirectory(Browser)