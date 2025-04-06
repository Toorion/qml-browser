# Contributing to QmlBrowser

Thank you for your interest in contributing to QmlBrowser! Your contributions are highly appreciated. Here are some guidelines to help you get started.

## How to Contribute

### Reporting Bugs

If you encounter any bugs while using QmlBrowser, please report them by opening an issue in the [issue tracker](https://github.com/Toorion/qml-browser/issues). Include as much detail as possible to help us diagnose and fix the issue quickly.

### Feature Requests

We welcome suggestions for new features! If you have an idea for a feature, please check the [open issues](https://github.com/Toorion/qml-browser/issues) to see if it has already been suggested. If not, feel free to open a new issue to discuss your idea.

### Submitting Changes

1. **Fork the repository**: Click on the "Fork" button at the top of this page to create a copy of the repository in your GitHub account.
2. **Clone the repository**: Clone the forked repository to your local machine using the following command:
   ```sh
   git clone https://github.com/your-username/qml-browser.git
   ```
3. **Create a new branch**: Create a new branch for your changes using the following command:
   ```sh
   git checkout -b feature-branch
   ```
4. **Make your changes**: Make your changes to the codebase.
5. **Commit your changes**: Commit your changes with a descriptive commit message using the following command:
   ```sh
   git commit -m "Description of your changes"
   ```
6. **Push your changes**: Push your changes to your forked repository using the following command:
   ```sh
   git push origin feature-branch
   ```
7. **Create a pull request**: Open a pull request from your forked repository to the main repository. Describe your changes and the problem they solve.

### Code Style

Please follow the existing code style and conventions in the project. This helps maintain consistency and readability throughout the codebase.

### Documentation

If your changes include new features or modifications, please update the relevant documentation in the `README.md` file or the [wiki](https://github.com/Toorion/qml-browser/wiki).

## Building the Project

Before building the browser core, you need to build the `libgit2` library:
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
Use QtCreator to build with the following kit:
- Qt + MSVC2019

## Contact

If you have any questions or need further assistance, feel free to reach out to us:
- **Email**: <toorion@gmail.com>
- **Telegram**: [@qmlbrowser](https://t.me/qmlbrowser)

Thank you for contributing to QmlBrowser!
