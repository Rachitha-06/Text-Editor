#include "../src/Document.h"
#include "../src/DoublyLinkedList.h"
#include "../src/Editor.h"
#include "../src/SearchEngine.h"
#include "../src/FileManager.h"
#include "../src/UndoRedoManager.h"
#include "../src/Command.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>

static int testsPassed = 0;
static int testsFailed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if (cond) { ++testsPassed; } \
        else { \
            ++testsFailed; \
            std::cerr << "FAIL: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        auto _a = (a); auto _b = (b); \
        if (_a == _b) { ++testsPassed; } \
        else { \
            ++testsFailed; \
            std::cerr << "FAIL: " << #a << " == " << #b \
                      << " (" << _a << " != " << _b << ") at " \
                      << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while (0)

void testDoublyLinkedList() {
    std::cout << "Testing DoublyLinkedList...\n";
    DoublyLinkedList list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);

    list.insert(0, "first");
    EXPECT_EQ(list.size(), 1u);
    EXPECT_EQ(list.at(0), "first");

    list.insert(1, "second");
    list.insert(2, "third");
    EXPECT_EQ(list.size(), 3u);
    EXPECT_EQ(list.at(0), "first");
    EXPECT_EQ(list.at(1), "second");
    EXPECT_EQ(list.at(2), "third");

    list.insert(1, "middle");
    EXPECT_EQ(list.size(), 4u);
    EXPECT_EQ(list.at(1), "middle");
    EXPECT_EQ(list.at(2), "second");

    list.remove(1);
    EXPECT_EQ(list.size(), 3u);
    EXPECT_EQ(list.at(1), "second");

    list.remove(0);
    EXPECT_EQ(list.at(0), "second");

    list.remove(list.size() - 1);
    EXPECT_EQ(list.size(), 1u);

    list.clear();
    EXPECT_TRUE(list.empty());
}

void testDocumentBasics() {
    std::cout << "Testing Document basics...\n";
    Document doc;
    EXPECT_TRUE(doc.empty());
    EXPECT_EQ(doc.lineCount(), 0u);

    doc.appendLine("Hello");
    doc.appendLine("World");
    EXPECT_EQ(doc.lineCount(), 2u);
    EXPECT_EQ(doc.getLine(0), "Hello");
    EXPECT_EQ(doc.getLine(1), "World");
    EXPECT_TRUE(doc.isModified());

    doc.insertLine(1, "Beautiful");
    EXPECT_EQ(doc.getLine(1), "Beautiful");
    EXPECT_EQ(doc.lineCount(), 3u);

    doc.deleteLine(1);
    EXPECT_EQ(doc.getLine(1), "World");

    doc.insertText(0, 5, " there");
    EXPECT_EQ(doc.getLine(0), "Hello there");

    doc.deleteText(0, 5, 6);
    EXPECT_EQ(doc.getLine(0), "Hello");

    auto stats = doc.calculateStats();
    EXPECT_EQ(stats.lines, 2u);
    EXPECT_TRUE(stats.words >= 2);
}

void testUndoRedo() {
    std::cout << "Testing Undo/Redo...\n";
    Editor editor;
    editor.newDocument();

    editor.appendLine("Line 1");
    editor.appendLine("Line 2");
    EXPECT_EQ(editor.document().lineCount(), 2u);
    EXPECT_TRUE(editor.canUndo());

    editor.undo();
    EXPECT_EQ(editor.document().lineCount(), 1u);
    EXPECT_TRUE(editor.canRedo());

    editor.redo();
    EXPECT_EQ(editor.document().lineCount(), 2u);

    editor.deleteLine(0);
    EXPECT_EQ(editor.document().getLine(0), "Line 2");

    editor.undo();
    EXPECT_EQ(editor.document().getLine(0), "Line 1");

    // New edit should clear redo
    editor.appendLine("Line 3");
    EXPECT_TRUE(!editor.canRedo());
}

void testSearchReplace() {
    std::cout << "Testing Search and Replace...\n";
    Editor editor;
    editor.newDocument();
    editor.appendLine("The quick brown fox");
    editor.appendLine("jumps over the lazy dog");
    editor.appendLine("The fox is quick");

    auto matches = editor.findAll("fox", true);
    EXPECT_EQ(matches.size(), 2u);

    matches = editor.findAll("the", false); // case-insensitive
    EXPECT_TRUE(matches.size() >= 2);

    bool replaced = editor.replaceFirst("fox", "cat", true);
    EXPECT_TRUE(replaced);
    EXPECT_EQ(editor.document().getLine(0), "The quick brown cat");

    std::size_t count = editor.replaceAll("quick", "fast", false);
    EXPECT_TRUE(count >= 1);
}

void testFileOperations() {
    std::cout << "Testing File operations...\n";
    const std::string testFile = "test_temp_lineforge.txt";
    const std::string testContent = "Line one\nLine two\nLine three";

    // Write a temp file
    {
        std::ofstream out(testFile);
        out << testContent;
    }

    Editor editor;
    auto result = editor.openFile(testFile);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(editor.document().lineCount(), 3u);
    EXPECT_EQ(editor.document().getLine(0), "Line one");
    EXPECT_EQ(editor.document().getLine(2), "Line three");
    EXPECT_TRUE(!editor.document().isModified());

    editor.appendLine("Line four");
    EXPECT_TRUE(editor.document().isModified());

    auto saveResult = editor.saveFile();
    EXPECT_TRUE(saveResult.success);
    EXPECT_TRUE(!editor.document().isModified());

    // Save As
    const std::string testFile2 = "test_temp_lineforge2.txt";
    auto saveAsResult = editor.saveFileAs(testFile2);
    EXPECT_TRUE(saveAsResult.success);
    EXPECT_EQ(editor.document().filename(), testFile2);

    // Cleanup
    std::remove(testFile.c_str());
    std::remove(testFile2.c_str());
}

void testKMP() {
    std::cout << "Testing KMP search...\n";
    auto positions = SearchEngine::kmpSearch("AABAACAADAABAABA", "AABA");
    EXPECT_TRUE(positions.size() >= 2);
    EXPECT_EQ(positions[0], 0u);

    // Overlapping pattern
    positions = SearchEngine::kmpSearch("AAAA", "AA");
    EXPECT_TRUE(positions.size() >= 2);
}

void testEdgeCases() {
    std::cout << "Testing edge cases...\n";
    Document doc;
    doc.setFromText("");
    EXPECT_TRUE(doc.empty());

    doc.setFromText("Only one line");
    EXPECT_EQ(doc.lineCount(), 1u);

    doc.clear();
    EXPECT_TRUE(doc.empty());

    // Empty search
    Editor editor;
    editor.newDocument();
    editor.appendLine("test");
    auto matches = editor.findAll("", true);
    EXPECT_EQ(matches.size(), 0u);

    // Out of range protections are exercised by the UI; here we just ensure no crash
    EXPECT_TRUE(!editor.insertLine(100, "bad"));
    EXPECT_TRUE(!editor.deleteLine(50));
}

int main() {
    std::cout << "===== LineForge Automated Tests =====\n\n";

    testDoublyLinkedList();
    testDocumentBasics();
    testUndoRedo();
    testSearchReplace();
    testFileOperations();
    testKMP();
    testEdgeCases();

    std::cout << "\n===== Results =====\n";
    std::cout << "Passed: " << testsPassed << "\n";
    std::cout << "Failed: " << testsFailed << "\n";

    if (testsFailed == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}
