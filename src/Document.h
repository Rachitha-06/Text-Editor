#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "DoublyLinkedList.h"
#include <string>
#include <vector>
#include <cstddef>

/**
 * Document represents the text content as a doubly linked list of lines.
 * Provides high-level editing operations and statistics.
 */
class Document {
public:
    Document();
    explicit Document(const std::string& filename);

    // Disable copy to avoid accidental expensive copies; provide explicit clone if needed
    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

    Document(Document&&) noexcept = default;
    Document& operator=(Document&&) noexcept = default;

    // Basic state
    bool empty() const;
    std::size_t lineCount() const;
    const std::string& filename() const;
    void setFilename(const std::string& name);
    bool isModified() const;
    void setModified(bool modified);

    // Line-level operations (0-based indices)
    void insertLine(std::size_t index, const std::string& text);
    void appendLine(const std::string& text);
    void deleteLine(std::size_t index);
    void replaceLine(std::size_t index, const std::string& text);
    const std::string& getLine(std::size_t index) const;
    std::string& getLine(std::size_t index);

    // Character / substring operations within a line
    void insertText(std::size_t lineIndex, std::size_t charPos, const std::string& text);
    void deleteText(std::size_t lineIndex, std::size_t startPos, std::size_t length);
    void replaceTextInLine(std::size_t lineIndex, std::size_t startPos, std::size_t length,
                           const std::string& replacement);

    // Whole document operations
    void clear();
    std::string getFullText() const;               // Joins lines with '\n'
    void setFromText(const std::string& text);     // Split by '\n'

    // Statistics
    struct Stats {
        std::size_t lines = 0;
        std::size_t characters = 0;      // Total chars excluding newlines
        std::size_t charactersWithNewlines = 0;
        std::size_t words = 0;
        std::size_t emptyLines = 0;
        std::size_t longestLineLength = 0;
        double averageLineLength = 0.0;
    };
    Stats calculateStats() const;

    // For internal / undo use
    DoublyLinkedList& lines() { return lines_; }
    const DoublyLinkedList& lines() const { return lines_; }

private:
    DoublyLinkedList lines_;
    std::string filename_;
    bool modified_;

    static std::size_t countWords(const std::string& s);
};

#endif // DOCUMENT_H
