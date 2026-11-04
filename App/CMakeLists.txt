cmake_minimum_required(VERSION 3.16)

# App library
qt_add_library(App SHARED
    app.cpp
    appdb.cpp
    apppaths.cpp
    mainapplication.cpp
    settings.cpp
)

# Headers for export
set(App_headers
    App_global.h
    app.h
    appdb.h
    apppaths.h
    mainapplication.h
    settings.h
)

# Qt dependencies
target_link_libraries(App PUBLIC
    Qt::Widgets
    Qt::Sql
)

# Include directories
target_include_directories(App PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/App>
)

# Compile definitions
target_compile_definitions(App PUBLIC APP_LIBRARY)

# Install
install(TARGETS App
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${App_headers} DESTINATION include/App)