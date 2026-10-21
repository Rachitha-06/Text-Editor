#include "UserInterface.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

UserInterface::UserInterface() {}

void UserInterface::clearScreen() {
#ifdef _WIN32
    (void)std::system("cls");
#else
    (void)std::system("clear");
#endif
}
void UserInterface::pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string UserInterface::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int UserInterface::readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        try {
            size_t pos = 0;
            int value = std::stoi(input, &pos);
            if (pos == input.size() && value >= minVal && value <= maxVal) {
                return value;
            }
        } catch (...) {}
        std::cout << "Invalid input. Please enter a number between "
                  << minVal << " and " << maxVal << ".\n";
    }
}

bool UserInterface::confirm(const std::string& question) {
    while (true) {
        std::string ans = readLine(question + " (y/n): ");
        if (ans == "y" || ans == "Y" || ans == "yes" || ans == "Yes") return true;
        if (ans == "n" || ans == "N" || ans == "no" || ans == "No") return false;
        std::cout << "Please answer y or n.\n";
    }
}

void UserInterface::printHeader() {
    std::cout << "================================================\n";
    std::cout << "              LineForge Text Editor\n";
    std::cout << "================================================\n";
}

void UserInterface::printDocumentStatus() {
    if (!editor_.hasDocument()) {
        std::cout << "[No document open]\n";
        return;
    }
    const auto& doc = editor_.document();
    std::cout << "Document: " << doc.filename();
    if (doc.isModified()) std::cout << " *";
    std::cout << "  |  Lines: " << doc.lineCount();
    auto s = doc.calculateStats();
    std::cout << "  |  Chars: " << s.characters
              << "  |  Words: " << s.words << "\n";
}

void UserInterface::displayDocument(std::size_t startLine, std::size_t maxLines) {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    const auto& doc = editor_.document();
    std::size_t total = doc.lineCount();
    if (total == 0) {
        std::cout << "(Empty document)\n";
        return;
    }
    if (startLine >= total) startLine = 0;
    std::size_t endLine = std::min(startLine + maxLines, total);

    std::cout << "\n----- Document Content (lines " << (startLine + 1)
              << "-" << endLine << " of " << total << ") -----\n";
    for (std::size_t i = startLine; i < endLine; ++i) {
        std::cout << std::setw(4) << (i + 1) << " | " << doc.getLine(i) << "\n";
    }
    if (endLine < total) {
        std::cout << "... (" << (total - endLine) << " more lines)\n";
    }
    std::cout << "-----------------------------------------------\n";
}

void UserInterface::handleNewDocument() {
    if (editor_.hasDocument() && editor_.document().isModified()) {
        if (!confirm("Current document has unsaved changes. Discard?")) {
            return;
        }
    }
    editor_.newDocument();
    std::cout << "New empty document created.\n";

    // Allow immediate text entry
    if (confirm("Enter text now? (lines will be added until you type :end on its own line)")) {
        std::cout << "Enter text (type :end on a line by itself to finish):\n";
        while (true) {
            std::string line = readLine("");
            if (line == ":end") break;
            editor_.appendLine(line);
        }
        std::cout << "Text entry finished. Lines: " << editor_.document().lineCount() << "\n";
    }
}

void UserInterface::handleOpenDocument() {
    if (editor_.hasDocument() && editor_.document().isModified()) {
        if (!confirm("Current document has unsaved changes. Discard?")) {
            return;
        }
    }
    std::string path = readLine("Enter file path to open: ");
    if (path.empty()) {
        std::cout << "Path cannot be empty.\n";
        return;
    }
    auto result = editor_.openFile(path);
    std::cout << result.message << "\n";
    if (result.success) {
        displayDocument();
    }
}

void UserInterface::handleViewDocument() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    const auto& doc = editor_.document();
    std::size_t total = doc.lineCount();
    if (total == 0) {
        std::cout << "(Empty document)\n";
        return;
    }

    if (total <= 30) {
        displayDocument(0, total);
    } else {
        std::cout << "Document has " << total << " lines.\n";
        int choice = readInt("1. View all\n2. View range\nChoice: ", 1, 2);
        if (choice == 1) {
            displayDocument(0, total);
        } else {
            int start = readInt("Start line (1-based): ", 1, static_cast<int>(total));
            int count = readInt("Number of lines to show: ", 1, 1000);
            displayDocument(static_cast<std::size_t>(start - 1), static_cast<std::size_t>(count));
        }
    }
}

void UserInterface::handleInsertText() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open. Create or open a document first.\n";
        return;
    }
    std::cout << "\nInsert options:\n";
    std::cout << "1. Insert new line at beginning\n";
    std::cout << "2. Append new line at end\n";
    std::cout << "3. Insert new line at specific position\n";
    std::cout << "4. Insert text inside an existing line\n";
    int choice = readInt("Choice: ", 1, 4);

    if (choice == 1) {
        std::string text = readLine("Text for new line: ");
        editor_.insertLine(0, text);
        std::cout << "Line inserted at beginning.\n";
    } else if (choice == 2) {
        std::string text = readLine("Text for new line: ");
        editor_.appendLine(text);
        std::cout << "Line appended.\n";
    } else if (choice == 3) {
        std::size_t n = editor_.document().lineCount();
        int idx = readInt("Insert before line number (1 to " + std::to_string(n + 1) + "): ",
                          1, static_cast<int>(n + 1));
        std::string text = readLine("Text for new line: ");
        editor_.insertLine(static_cast<std::size_t>(idx - 1), text);
        std::cout << "Line inserted.\n";
    } else {
        std::size_t n = editor_.document().lineCount();
        if (n == 0) {
            std::cout << "Document is empty. Insert a line first.\n";
            return;
        }
        int lineNum = readInt("Line number (1-based): ", 1, static_cast<int>(n));
        const std::string& line = editor_.document().getLine(static_cast<std::size_t>(lineNum - 1));
        int pos = readInt("Character position (0 to " + std::to_string(line.size()) + "): ",
                          0, static_cast<int>(line.size()));
        std::string text = readLine("Text to insert: ");
        if (editor_.insertText(static_cast<std::size_t>(lineNum - 1),
                               static_cast<std::size_t>(pos), text)) {
            std::cout << "Text inserted.\n";
        } else {
            std::cout << "Insert failed (invalid position).\n";
        }
    }
}

void UserInterface::handleDeleteText() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    std::size_t n = editor_.document().lineCount();
    if (n == 0) {
        std::cout << "Document is empty.\n";
        return;
    }

    std::cout << "\nDelete options:\n";
    std::cout << "1. Delete an entire line\n";
    std::cout << "2. Delete characters inside a line\n";
    int choice = readInt("Choice: ", 1, 2);

    if (choice == 1) {
        int lineNum = readInt("Line number to delete (1-based): ", 1, static_cast<int>(n));
        if (confirm("Really delete line " + std::to_string(lineNum) + "?")) {
            editor_.deleteLine(static_cast<std::size_t>(lineNum - 1));
            std::cout << "Line deleted.\n";
        }
    } else {
        int lineNum = readInt("Line number (1-based): ", 1, static_cast<int>(n));
        const std::string& line = editor_.document().getLine(static_cast<std::size_t>(lineNum - 1));
        if (line.empty()) {
            std::cout << "Line is empty.\n";
            return;
        }
        int start = readInt("Start position (0-based): ", 0, static_cast<int>(line.size() - 1));
        int len = readInt("Number of characters to delete: ", 1,
                          static_cast<int>(line.size() - start));
        if (confirm("Delete " + std::to_string(len) + " character(s)?")) {
            if (editor_.deleteText(static_cast<std::size_t>(lineNum - 1),
                                   static_cast<std::size_t>(start),
                                   static_cast<std::size_t>(len))) {
                std::cout << "Text deleted.\n";
            } else {
                std::cout << "Delete failed.\n";
            }
        }
    }
}

void UserInterface::handleFindText() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    std::string pattern = readLine("Search for: ");
    if (pattern.empty()) {
        std::cout << "Search pattern cannot be empty.\n";
        return;
    }
    bool caseSensitive = confirm("Case-sensitive search?");

    auto matches = editor_.findAll(pattern, caseSensitive);
    if (matches.empty()) {
        std::cout << "No matches found.\n";
        return;
    }
    std::cout << "Found " << matches.size() << " match(es):\n";
    for (const auto& m : matches) {
        std::cout << "  Line " << (m.lineIndex + 1) << ", position " << m.position
                  << ": " << editor_.document().getLine(m.lineIndex) << "\n";
    }
}

void UserInterface::handleReplaceText() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    std::string pattern = readLine("Search for: ");
    if (pattern.empty()) {
        std::cout << "Search pattern cannot be empty.\n";
        return;
    }
    std::string replacement = readLine("Replace with: ");
    bool caseSensitive = confirm("Case-sensitive?");

    std::cout << "1. Replace first occurrence only\n";
    std::cout << "2. Replace all occurrences\n";
    int choice = readInt("Choice: ", 1, 2);

    if (choice == 1) {
        if (editor_.replaceFirst(pattern, replacement, caseSensitive)) {
            std::cout << "First occurrence replaced.\n";
        } else {
            std::cout << "No match found.\n";
        }
    } else {
        std::size_t count = editor_.replaceAll(pattern, replacement, caseSensitive);
        std::cout << count << " occurrence(s) replaced.\n";
    }
}

void UserInterface::handleUndo() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    if (!editor_.canUndo()) {
        std::cout << "Nothing to undo.\n";
        return;
    }
    std::string desc = editor_.undoDescription();
    if (editor_.undo()) {
        std::cout << "Undone: " << desc << "\n";
    } else {
        std::cout << "Undo failed.\n";
    }
}

void UserInterface::handleRedo() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    if (!editor_.canRedo()) {
        std::cout << "Nothing to redo.\n";
        return;
    }
    std::string desc = editor_.redoDescription();
    if (editor_.redo()) {
        std::cout << "Redone: " << desc << "\n";
    } else {
        std::cout << "Redo failed.\n";
    }
}

void UserInterface::handleSave() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    if (editor_.document().filename() == "Untitled") {
        std::cout << "Document has no filename yet. Use Save As.\n";
        handleSaveAs();
        return;
    }
    auto result = editor_.saveFile();
    std::cout << result.message << "\n";
}

void UserInterface::handleSaveAs() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    std::string path = readLine("Enter new file path: ");
    if (path.empty()) {
        std::cout << "Path cannot be empty.\n";
        return;
    }
    if (FileManager::fileExists(path)) {
        if (!confirm("File already exists. Overwrite?")) {
            std::cout << "Save As cancelled.\n";
            return;
        }
    }
    auto result = editor_.saveFileAs(path);
    std::cout << result.message << "\n";
}

void UserInterface::handleStatistics() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    auto s = editor_.stats();
    std::cout << "\n----- Document Statistics -----\n";
    std::cout << "Lines:              " << s.lines << "\n";
    std::cout << "Characters:         " << s.characters
              << " (excluding newlines)\n";
    std::cout << "Chars + newlines:   " << s.charactersWithNewlines << "\n";
    std::cout << "Words:              " << s.words << "\n";
    std::cout << "Empty lines:        " << s.emptyLines << "\n";
    std::cout << "Longest line:       " << s.longestLineLength << " chars\n";
    std::cout << "Average line length:" << std::fixed << std::setprecision(1)
              << s.averageLineLength << " chars\n";
    std::cout << "-------------------------------\n";
    std::cout << "Note: Character counts treat the text as a sequence of bytes/chars\n";
    std::cout << "as stored by std::string (UTF-8 multibyte characters count as multiple).\n";
}

void UserInterface::handleCloseDocument() {
    if (!editor_.hasDocument()) {
        std::cout << "No document is open.\n";
        return;
    }
    if (editor_.document().isModified()) {
        if (!confirm("Document has unsaved changes. Close anyway?")) {
            return;
        }
    }
    editor_.closeDocument();
    std::cout << "Document closed.\n";
}

void UserInterface::documentMenu() {
    while (editor_.hasDocument()) {
        clearScreen();
        printHeader();
        printDocumentStatus();
        std::cout << "\n----- Document Menu -----\n";
        std::cout << " 1. View document\n";
        std::cout << " 2. Insert text\n";
        std::cout << " 3. Delete text\n";
        std::cout << " 4. Find text\n";
        std::cout << " 5. Replace text\n";
        std::cout << " 6. Undo";
        if (editor_.canUndo()) std::cout << " (" << editor_.undoDescription() << ")";
        std::cout << "\n";
        std::cout << " 7. Redo";
        if (editor_.canRedo()) std::cout << " (" << editor_.redoDescription() << ")";
        std::cout << "\n";
        std::cout << " 8. Save\n";
        std::cout << " 9. Save As\n";
        std::cout << "10. Document statistics\n";
        std::cout << "11. Close document\n";
        std::cout << " 0. Back to main menu\n";
        std::cout << "-------------------------\n";

        int choice = readInt("Choice: ", 0, 11);
        switch (choice) {
            case 1: handleViewDocument(); break;
            case 2: handleInsertText(); break;
            case 3: handleDeleteText(); break;
            case 4: handleFindText(); break;
            case 5: handleReplaceText(); break;
            case 6: handleUndo(); break;
            case 7: handleRedo(); break;
            case 8: handleSave(); break;
            case 9: handleSaveAs(); break;
            case 10: handleStatistics(); break;
            case 11: handleCloseDocument(); break;
            case 0:
                if (editor_.hasDocument() && editor_.document().isModified()) {
                    if (!confirm("Document has unsaved changes. Leave anyway?")) {
                        continue;
                    }
                }
                return;
        }
        if (choice != 0 && choice != 11) pause();
    }
}

void UserInterface::mainMenu() {
    while (true) {
        clearScreen();
        printHeader();
        printDocumentStatus();
        std::cout << "\n----- Main Menu -----\n";
        std::cout << "1. Create new document\n";
        std::cout << "2. Open existing document\n";
        if (editor_.hasDocument()) {
            std::cout << "3. Work with current document\n";
        }
        std::cout << "0. Exit\n";
        std::cout << "---------------------\n";

        int maxChoice = editor_.hasDocument() ? 3 : 2;
        int choice = readInt("Choice: ", 0, maxChoice);

        switch (choice) {
            case 1:
                handleNewDocument();
                if (editor_.hasDocument()) {
                    pause();
                    documentMenu();
                }
                break;
            case 2:
                handleOpenDocument();
                if (editor_.hasDocument()) {
                    pause();
                    documentMenu();
                } else {
                    pause();
                }
                break;
            case 3:
                if (editor_.hasDocument()) documentMenu();
                break;
            case 0:
                if (editor_.hasDocument() && editor_.document().isModified()) {
                    if (!confirm("Unsaved changes will be lost. Exit anyway?")) {
                        continue;
                    }
                }
                std::cout << "Goodbye!\n";
                return;
        }
    }
}

void UserInterface::run() {
    mainMenu();
}
