# LineForge

**A lightweight, console-based text editor written in modern C++**

LineForge is a fully functional menu-driven text editor designed as a Computer Science Engineering portfolio / academic project. It demonstrates practical use of **doubly linked lists**, **stack-based undo/redo (Command pattern)**, file handling, search & replace algorithms (including optional KMP), and clean modular design.

---

## Features

- Create, open, save, and Save As text documents
- Multi-line editing with line-number display
- Insert lines or text at arbitrary positions
- Delete lines or character ranges
- Find (case-sensitive / case-insensitive)
- Replace first / Replace all
- Full undo / redo history using the Command pattern
- Document statistics (lines, characters, words, longest line, etc.)
- Input validation and graceful error handling
- Pagination for long documents
- Unsaved-change detection

## Technologies

| Component          | Choice                          |
|--------------------|---------------------------------|
| Language           | C++17                           |
| Interface          | Interactive console             |
| Data structure     | Custom doubly linked list       |
| Undo/Redo          | Command pattern + stacks        |
| Search             | Substring + optional KMP        |
| Build              | CMake + Makefile                |
| Testing            | Custom automated test suite     |

## Project Structure

```
LineForge/
├── README.md
├── LICENSE
├── .gitignore
├── CMakeLists.txt
├── Makefile
├── src/
│   ├── main.cpp
│   ├── DoublyLinkedList.{h,cpp}
│   ├── Document.{h,cpp}
│   ├── Command.{h,cpp}
│   ├── UndoRedoManager.{h,cpp}
│   ├── SearchEngine.{h,cpp}
│   ├── FileManager.{h,cpp}
│   ├── Editor.{h,cpp}
│   └── UserInterface.{h,cpp}
├── tests/
│   └── test_main.cpp
├── examples/
│   ├── sample.txt
│   └── README.md
├── docs/
│   ├── project_report.md
│   ├── data_structures.md
│   ├── algorithms.md
│   ├── complexity_analysis.md
│   ├── test_cases.md
│   ├── test_results.md
│   ├── viva_questions.md
│   ├── viva_answers.md
│   └── ...
└── screenshots/
    └── README.md
```

## Prerequisites

- A C++17 compatible compiler (g++, clang++, or MSVC)
- Optional: CMake ≥ 3.10

## Compilation

### Using Makefile (recommended for quick start)

**Linux / macOS / WSL / MinGW:**

```bash
cd LineForge
make
```

This produces the executable `lineforge`.

**Run the automated tests:**

```bash
make test
# or
./lineforge_tests
```

**Clean:**

```bash
make clean
```

### Using CMake

```bash
cd LineForge
mkdir build && cd build
cmake ..
cmake --build .
./lineforge          # or lineforge.exe on Windows
./lineforge_tests
```

### Windows (Command Prompt / PowerShell with MinGW or MSVC)

If you have MinGW-w64 or MSYS2:

```cmd
cd LineForge
mingw32-make
lineforge.exe
```

With Visual Studio / MSVC (Developer Command Prompt):

```cmd
cd LineForge
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Usage

```bash
./lineforge
```

Follow the on-screen menus:

1. Create a new document or open an existing file
2. Use the Document Menu to view, insert, delete, find, replace, undo, redo, save, etc.
3. Exit cleanly; the program warns about unsaved changes

### Example session

```
================================================
              LineForge Text Editor
================================================
[No document open]

----- Main Menu -----
1. Create new document
2. Open existing document
0. Exit
```

Open `examples/sample.txt`, try searching for “fox”, replace text, undo, and save a copy.

## Testing

The project includes a self-contained test suite covering:

- Doubly linked list operations
- Document insertion / deletion
- Undo / redo sequences
- Search and replace (including case-insensitive)
- File load / save / Save As
- KMP pattern matching
- Edge cases (empty document, empty search, out-of-range)

All 61 automated assertions currently pass.

```bash
./lineforge_tests
```

## Known Limitations

- Console interface only (no GUI)
- UTF-8 is stored and written correctly, but terminal display and input of multi-byte characters depend on the user’s terminal/locale
- Character counts treat `std::string` bytes as characters (no full Unicode grapheme clustering)
- Large files are limited by available memory (the whole document is kept in RAM)
- No syntax highlighting or multi-buffer support in this version

## Future Enhancements

- Optional GUI frontend (Qt / Dear ImGui) reusing the same core
- Multiple open documents / tabs
- Syntax highlighting for common languages
- Configurable key bindings
- Persistent configuration file
- More advanced search (regex)

## License

MIT License – see [LICENSE](LICENSE).

## Author

Computer Science Engineering student project – built for learning data structures, algorithms, and clean C++ architecture.
