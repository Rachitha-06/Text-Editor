#ifndef EDITOR_H
#define EDITOR_H

#include "Document.h"
#include "UndoRedoManager.h"
#include "SearchEngine.h"
#include "FileManager.h"
#include <memory>
#include <string>
#include <vector>

/**
 * High-level editor that owns the document and undo/redo manager.
 * Provides a clean API used by the UserInterface.
 */
class Editor {
public:
    Editor();

    Document& document();
    const Document& document() const;

    bool hasDocument() const;
    void newDocument();
    void closeDocument();

    // File operations
    FileManager::Result openFile(const std::string& path);
    FileManager::Result saveFile();
    FileManager::Result saveFileAs(const std::string& path);

    // Editing (these create and execute commands)
    bool insertLine(std::size_t index, const std::string& text);
    bool appendLine(const std::string& text);
    bool deleteLine(std::size_t index);
    bool insertText(std::size_t lineIndex, std::size_t charPos, const std::string& text);
    bool deleteText(std::size_t lineIndex, std::size_t startPos, std::size_t length);

    // Search / Replace
    std::vector<SearchEngine::Match> findAll(const std::string& pattern, bool caseSensitive = true);
    bool replaceFirst(const std::string& pattern, const std::string& replacement,
                      bool caseSensitive = true);
    std::size_t replaceAll(const std::string& pattern, const std::string& replacement,
                           bool caseSensitive = true);

    // Undo / Redo
    bool undo();
    bool redo();
    bool canUndo() const;
    bool canRedo() const;
    std::string undoDescription() const;
    std::string redoDescription() const;

    // Statistics
    Document::Stats stats() const;

private:
    std::unique_ptr<Document> doc_;
    UndoRedoManager history_;
};

#endif // EDITOR_H
