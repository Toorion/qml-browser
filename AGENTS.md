# QmlBrowser - Agent Instructions

## Project Overview
Qt/QML-based Web 3.0 browser with QML page support (SDAPPS). Uses Qt WebEngine, Quick3D, and libgit2.

## Build System
- **qmake** (subdirs template), not CMake
- Top-level: `QmlBrowser.pro` → builds `App` → `Net` → `Api` → `Browser` (ordered)
- C++11, `CONFIG -= debug_and_release`

## Prerequisites
1. **libgit2 submodule** must be built first:
   ```sh
   git submodule init && git submodule update
   cd 3rdparty/libgit2 && mkdir build && cd build
   cmake .. && cmake --build . --config Release
   ```

## Build Commands

### Linux
```sh
mkdir build && cd build
qmake ..
make && make install
```

### Windows
Use QtCreator with **Qt + MSVC2019** kit.

## Module Dependencies
```
App (lib) → Net (lib) → Api (lib) → Browser (app)
```
- `App`: Core app logic, DB, settings
- `Net`: Network stack, download manager, libgit2 integration
- `Api`: QML/JS API, bookmarks, history, downloads
- `Browser`: Main window, tabs, WebEngine views, UI

## Key Qt Modules Used
- `widgets`, `sql`, `network`, `qml`, `quick`, `webenginewidgets`, `quickcontrols2`, `quick3d`, `concurrent`
- Private modules: `widgets-private`, `core-private`, `network-private`

## Deployment
- **CQtDeployer** config: `CQtDeployer.json`
- Output: `../QmlBrowser-deploy`
- Extra assets: `Browser/styles.qss`, `Browser/icons/`, `Api/tools/`

## Code Conventions
- Header guards: `*_GLOBAL_H` for library exports
- Private headers: `*_p.h`
- Library macros: `APP_LIBRARY`, `NET_LIBRARY`, `API_LIBRARY`

## Testing
No automated test suite detected. Manual testing via built browser.

## Environment
- Qt 6.8+ (CQtDeployer references 6.8.1)
- libgit2 (submodule)
- Linux: gcc, qmake, make
- Windows: MSVC2019, QtCreator