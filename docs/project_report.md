# Project Report: LineForge – A Console Text Editor in C++

## Abstract

LineForge is a fully functional, menu-driven console text editor implemented in C++17. It uses a custom doubly linked list to represent document lines, the Command pattern with stacks for unlimited undo/redo, and standard file streams for persistence. The project demonstrates object-oriented design, data structures, algorithms, modular architecture, input validation, and automated testing. All core features have been implemented, compiled, and verified by a suite of 61 automated tests.

## 1. Introduction

Text editors are excellent vehicles for practising fundamental Computer Science concepts. This project implements a practical editor that students can compile, run, extend, and present in a viva.

## 2. Problem Statement

Design and implement a lightweight text editor that supports the essential operations of creating, opening, editing, searching, replacing, saving documents, and undoing/redoing changes, while clearly illustrating the use of doubly linked lists and stack-based history.

## 3. Objectives

- Provide a usable console text editor.
- Demonstrate a custom doubly linked list of lines.
- Implement genuine multi-level undo and redo via the Command pattern.
- Support search and replace (simple + KMP demonstration).
- Handle files safely with proper error reporting.
- Deliver clean modular source code, automated tests, and comprehensive documentation suitable for a CSE portfolio and viva.

## 4. Scope

In scope: console UI, single document, line- and character-level editing, undo/redo, find/replace, statistics, file I/O.  
Out of scope for this version: GUI, multi-document tabs, syntax highlighting, regular expressions, collaborative editing.

## 5. Requirements

See the feature list in the main README and the original project specification. All listed core features have been implemented.

## 6. System Architecture

Six main modules communicate through well-defined interfaces:

- **DoublyLinkedList** – low-level line storage
- **Document** – high-level content + statistics
- **Command hierarchy** – reversible operations
- **UndoRedoManager** – history stacks
- **SearchEngine** – find / replace / KMP
- **FileManager** – load / save
- **Editor** – façade that coordinates the above
- **UserInterface** – menus and input validation

## 7. Data Structures & Algorithms

Detailed in `docs/data_structures.md` and `docs/algorithms.md`.  
Primary structure: doubly linked list of lines.  
History: two stacks of `unique_ptr<Command>`.  
Search: linear scan + optional KMP.

## 8. Implementation Highlights

- RAII and `unique_ptr` for automatic resource management.
- Move-only Document and list to prevent accidental expensive copies.
- Replace-All collects matches first and applies them from the end, guaranteeing termination even when the replacement contains the search pattern.
- All user input is validated; invalid values never crash the program.
- Unsaved-change detection on close / new / open / exit.

## 9. Testing

A self-contained test suite exercises every major component.  
Result: **61 assertions passed, 0 failed** (see `docs/test_results.md`).

## 10. Limitations

- Console only.
- Character counts are byte-oriented (UTF-8 multi-byte characters count as multiple units).
- Whole document resides in memory.
- No syntax highlighting or multi-buffer support.

## 11. Future Enhancements

- GUI front-end reusing the same core.
- Multiple open documents.
- Regular-expression search.
- Configurable key bindings and themes.

## 12. Conclusion

LineForge successfully realises a complete, tested, and documented console text editor that fulfils the academic requirements while remaining understandable and extensible. The combination of a classic data structure (doubly linked list), a modern design pattern (Command), and thorough testing makes it a strong portfolio piece.

## References

- C++17 Standard
- “Design Patterns” (Gamma et al.) – Command pattern
- Classic algorithms texts for KMP
- ncurses / terminal handling literature (not used; pure ANSI / standard I/O chosen for portability)
