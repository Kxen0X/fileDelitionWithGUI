# File Deletion Tool

A Windows-oriented desktop utility built with **C++17 and Qt Widgets**. Select a directory, enter a base name, and search its subdirectories for matching files and folders before confirming deletion.

The final version from `dev` is now available in **`main`**.

## Features

- Directory selection through a native folder dialog.
- Recursive traversal with `QDirIterator`, including hidden entries.
- Exact or substring matching against `QFileInfo::baseName()`, selected with a checkbox.
- Input validation and an explicit confirmation dialog.
- Batch-script deletion of matching files and folders, including non-empty folders.
- A visible progress indicator and a final count of objects that could not be deleted.

**Deletion is permanent.** The bundled `toAdmin.bat` uses `takeown` and `icacls` to attempt ownership/permission changes, then `del` or recursive `rd /s /q` to remove the target. It does not use the Recycle Bin. Test only on disposable data that you own; do not select system directories.

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
4. Build the `schoolProject` target.
5. Copy `toAdmin.bat` from the repository root beside the generated executable, then run it. Embedding the script in `toAdmin.qrc` does not create the external file that `QProcess` expects.

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
copy ..\toAdmin.bat release\toAdmin.bat
release\schoolProject.exe
```

Keep the matching Qt runtime libraries available when launching outside Qt Creator.

## Usage

1. Choose a disposable directory with **«Выбрать директорию»**.
2. Enter the base name to match in **«Введите имя файла(ов)»**. For example, `report` matches a file named `report.txt`.
3. Fill in **«Введите имя пользователя»**. This value is used as a name exclusion during traversal. The batch script itself uses the current Windows `%USERNAME%`; the field is not an authentication mechanism.
4. Leave the matching checkbox unchecked for exact names, or check it for substring matches.
5. Click **«Удалить файлы»** and review the confirmation dialog.
6. Confirm only when every matching object and the contents of matching folders may be removed.

Matching uses Qt's `baseName()` behavior; there is no wildcard syntax or selectable results preview. The script can recursively remove the full contents of matching folders. Permission-changing commands may fail without sufficient privileges, and the application does not provide a UAC elevation flow.

## Code structure

| File | Responsibility |
| --- | --- |
| [main.cpp](main.cpp) | Qt application entry point |
| [mainwindow.cpp](mainwindow.cpp) | Input checks, folder selection, search, and deletion |
| [mainwindow.h](mainwindow.h) | Main window declaration |
| [mainwindow.ui](mainwindow.ui) | Qt Designer form |
| [schoolProject.pro](schoolProject.pro) | qmake build configuration |
| [toAdmin.bat](toAdmin.bat) | Permission changes and file/folder deletion |
| [toAdmin.qrc](toAdmin.qrc) | Embedded Qt resource declaration |

## Current limitations

The interface and path validation are Windows-oriented. Search and deletion run on the UI thread, with calls to `processEvents()` during traversal. External processes are awaited synchronously. Script deployment and launch behavior should be checked on the target Windows environment.

The code includes a few path/name exclusions, but they are not a comprehensive protection mechanism. A preview list, Recycle Bin support, background processing, and a visible results summary would make the next version more usable.
