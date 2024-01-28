# QmlBrowser

QmlBrowser is full-featured Chromium based open-source web browser
with QML SDAPPS (Serverless Decentralized Application) support.

It is next generation of Web 3.0 browser.

It use modern QML language to fill the pages with incredible features.

It allow running QML pages/SDAPPS installed directly from Git or filesystem

It support 3D functionality in the core and can render it fast on any hardware.

![QmlBrowser is full-featured Chromium based open-source web browser with QML pages support](preview.png)


## Downloads

Available on [Release](https://github.com/Toorion/qml-browser/releases) page

## Building

Before build browser core libgit2 should be builded:

```sh
git submodule init
git submodule update
cd BROWSER_SRC/3rdparty/libgit2
mkdir build && cd build
cmake ..
cmake --build .
```

### Linux
```sh
mkdir build && cd build
qmake ..
make && make install
```

### Windows

Use QtCreator to build 
Kit QT + MSVC2019

## Demo

[QML Demo page](http://demo.qbqsoft.com)

[QML Demo source *](https://github.com/Toorion/qml-browser-demo)

\* Can only be opened using QmlBrowser

## Open QML pages

For opening QML pages from Web it should have 

Content-Type: text/qml

## Install and use SDAPPS

For installing SDAPPS read 
[SDAPPS installation instruction](https://github.com/Toorion/qml-browser/wiki/SDAPPS)
in the Wiki

(in testing feature)

## 3D Advanced Configuration

For advanced 3d configuration read 
[Advanced 3D configuration](https://github.com/Toorion/qml-browser/wiki/3D-Config)
in the Wiki

## QML advantages over HTML

|           |QML              |  HTML   |
|:----------|:---------------:|:-------:|
| Oriented to |  2D / 3D | Text markup |
| Extendable |  Yes | No |
| Reusable |  Yes | No |
| Flexibility | High | Low |
| 3D performance | Fast | Slow |
| Development time |  Low | High |


* HTML describes how to structure text documents and links on the web. Overtime it was extended with JS and CSS to let you do much more but at it's core it is a text document rendering technology.
* QML was designed to describe how 2D and 3D spacial elements relate and interact with each other. It can still render text but that is not it's focus. QML is much more concerned with creating a user experience rather than presenting textual information.

## Contact

Email: <toorion@gmail.com>

Telegram: [@qmlbrowser](https://t.me/qmlbrowser)

## ToDo

- SDAPPS manager
- SDAPPS access controll
- Bookmarks
- Network adapter for QML mode
- Expand API model for QML pages
- IPFS Support
- Lot of revolution features and external integrations

