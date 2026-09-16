# File Deletion Tool (GUI) (look in dev branch)

A clean and lightweight desktop graphical application designed for quick and convenient file and directory removal.

---

## 📑 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [Tech Stack & Prerequisites](#-tech-stack--prerequisites)
- [Building & Setup](#-building--setup)
- [Usage](#-usage)

---

## 🚀 Overview

`fileDelitionWithGUI` provides a straightforward graphical user interface (GUI) to simplify file management and disk cleanup tasks. Users can select, review, and delete target files or entire folders without relying on command-line prompts.

---

## ✨ Key Features

* **Graphical Interface:** Easy-to-use desktop window for file and folder selection.
* **Batch Removal:** Select and delete multiple items at once.
* **Deletion Status:** Clear feedback on operation progress and completion.

---

## 🛠 Tech Stack & Prerequisites

* **Language:** C++
* **UI Framework:** WinAPI / Native GUI Framework
* **Compiler / IDE:** Visual Studio (MSVC) or MinGW (`g++`)

---

## ⚙️ Building & Setup

### Option 1: Visual Studio (Recommended)

1. Clone the repository:
   ```bash
   git clone https://github.com/Kxen0X/fileDelitionWithGUI.git
   ```
2. Open `fileDelitionWithGUI.sln` (or project file) in Visual Studio.
3. Select **Release** configuration and build the solution (`Ctrl + Shift + B`).

### Option 2: MinGW / Direct Compilation (`g++`)

```bash
g++ main.cpp -o fileDelitionWithGUI -mwindows
```

---

## 💻 Usage

Run the compiled executable directly from your file explorer or via terminal:

```cmd
.\fileDelitionWithGUI.exe
```
