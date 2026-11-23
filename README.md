# File Deletion Tool

A Windows-oriented desktop utility built with **C++17 and Qt Widgets**. Select a directory, enter a base name, and search its subdirectories for matching files and folders before confirming deletion.

This README describes the default **`main` branch**. A separate [`dev` branch](https://github.com/Kxen0X/fileDelitionWithGUI/tree/dev) also exists and may contain different behavior.

## Features

- Directory selection through a native folder dialog.
- Recursive traversal with `QDirIterator`, including hidden entries.
- Exact, case-sensitive matching against `QFileInfo::baseName()`.
- Input validation and an explicit confirmation dialog.
- Deletion of matching files and empty directories through Qt's filesystem APIs.

**Deletion is permanent:** the current implementation uses `QFile::remove()` and `QDir::rmdir()`, rather than moving items to the Recycle Bin. Try the application on a disposable test directory first.

## Requirements

- Windows with a C++17-capable compiler.
- Qt 6 with the Widgets module and a matching compiler kit.
- Qt Creator, or `qmake` plus the build tool for your Qt kit.

The checked-in build directory was generated with Qt 6.8.1 and MinGW. Use your own matching Qt/compiler kit and create a fresh build directory; the generated files are machine-specific.

## Build with Qt Creator

```sh
git clone https://github.com/Kxen0X/fileDelitionWithGUI.git
```

1. Open [schoolProject.pro](schoolProject.pro) in Qt Creator.
2. Select a desktop Qt kit with C++17 support.
3. Choose a new shadow-build directory.
4. Build and run the `schoolProject` target.

Build note: the current constructor passes `open_button` directly to three `connect()` calls. If your compiler rejects those member-function arguments, use `&MainWindow::open_button` for each connection.

The project uses **qmake**, not CMake or a Visual Studio solution. Compiling `main.cpp` alone will not generate the Qt UI and meta-object code or link the required Qt libraries.

### Command line with a Qt MinGW kit

Run these commands in the command prompt supplied with your Qt MinGW kit, where `qmake` and `mingw32-make` are on `PATH`:

```bat
cd fileDelitionWithGUI
mkdir build-local
cd build-local
qmake ..\schoolProject.pro "CONFIG+=release" "CONFIG-=debug"
mingw32-make
release\schoolProject.exe
```

Keep the matching Qt runtime libraries available when launching outside Qt Creator.

## Usage

1. Choose a disposable directory with **«Выбрать директорию»**.
2. Enter the base name to match in **«Введите имя файла(ов)»**. For example, `report` matches a file named `report.txt`.
3. Fill in **«Введите имя пользователя»**. This value is used as a name exclusion during traversal; it is not an authentication field.
4. Click **«Удалить файлы»** and review the confirmation dialog.
5. Confirm only when every matching object under the chosen directory may be removed.

Matching uses Qt's `baseName()` behavior, not a wildcard or substring search. There is no selectable results preview. Non-empty directories are not recursively removed by `rmdir()`.

## Code structure

| File | Responsibility |
| --- | --- |
| [main.cpp](main.cpp) | Qt application entry point |
| [mainwindow.cpp](mainwindow.cpp) | Input checks, folder selection, search, and deletion |
| [mainwindow.h](mainwindow.h) | Main window declaration |
| [mainwindow.ui](mainwindow.ui) | Qt Designer form |
| [schoolProject.pro](schoolProject.pro) | qmake build configuration |

## Current limitations

The interface and path validation are Windows-oriented. Search and deletion run on the UI thread, with calls to `processEvents()` during traversal. A progress bar is updated in code but is hidden in the current `main` implementation, and deletion failures are not summarized in the interface.

The code includes a few path/name exclusions, but they are not a comprehensive protection mechanism. A preview list, Recycle Bin support, background processing, and a visible results summary would make the next version more usable.
