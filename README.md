# QmlBrowser

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://paypal.me/toorion)

QmlBrowser is the next generation Web 3.0 browser with QML support, enabling the creation of SDAPPS (serverless decentralized applications) and Hybrid Client

**Key features:**

* Accepts QML pages (modern QML language) in the same way as HTML
* Unrestricted local storage per APP
* Ability to install APP locally without being bound to a Web server.
* An alternative security principle without the use of Cookies and CORS, which allows the application to interact with the Internet without restrictions.
* Support for 3D functionality directly in the core


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
cmake --build . --config Release
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

http[]()://demo.qbqsoft.com *

[QML Demo source](https://github.com/Toorion/qml-browser-demo)

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
- Network adapter for QML mode
- Expand API model for QML pages
- IPFS Support
- Lot of revolution features and external integrations

