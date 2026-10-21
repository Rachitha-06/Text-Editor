#ifndef COMMAND_H
#define COMMAND_H

#include "Document.h"
#include <memory>
#include <string>
#include <vector>

/**
 * Abstract base for undoable commands (Command pattern).
 * Each concrete command records enough state to execute and reverse itself.
 */
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const = 0;
};

// Insert a whole line at index
class InsertLineCommand : public Command {
public:
    InsertLineCommand(Document& doc, std::size_t index, const std::string& text);
    void execute() override;
    void undo() override;
    std::string description() const override;

private:
    Document& doc_;
    std::size_t index_;
    std::string text_;
    bool executed_ = false;
};

// Delete a whole line at index
class DeleteLineCommand : public Command {
public:
    DeleteLineCommand(Document& doc, std::size_t index);
    void execute() override;
    void undo() override;
    std::string description() const override;

private:
    Document& doc_;
    std::size_t index_;
    std::string deletedText_;
    bool executed_ = false;
};

// Insert text inside a line
class InsertTextCommand : public Command {
public:
    InsertTextCommand(Document& doc, std::size_t lineIndex, std::size_t charPos,
                      const std::string& text);
    void execute() override;
    void undo() override;
    std::string description() const override;

private:
    Document& doc_;
    std::size_t lineIndex_;
    std::size_t charPos_;
    std::string text_;
    bool executed_ = false;
};

// Delete a range of characters inside a line
class DeleteTextCommand : public Command {
public:
    DeleteTextCommand(Document& doc, std::size_t lineIndex, std::size_t startPos,
                      std::size_t length);
    void execute() override;
    void undo() override;
    std::string description() const override;

private:
    Document& doc_;
    std::size_t lineIndex_;
    std::size_t startPos_;
    std::size_t length_;
    std::string deletedText_;
    bool executed_ = false;
};

// Replace text in a line (used by search-replace for single occurrence)
class ReplaceTextCommand : public Command {
public:
    ReplaceTextCommand(Document& doc, std::size_t lineIndex, std::size_t startPos,
                       std::size_t length, const std::string& replacement);
    void execute() override;
    void undo() override;
    std::string description() const override;

private:
    Document& doc_;
    std::size_t lineIndex_;
    std::size_t startPos_;
    std::size_t oldLength_;
    std::string oldText_;
    std::string newText_;
    bool executed_ = false;
};

// Batch of replacements (for Replace All) - treats the whole batch as one undo unit
class ReplaceAllCommand : public Command {
public:
    struct Replacement {
        std::size_t lineIndex;
        std::size_t startPos;
        std::size_t oldLength;
        std::string oldText;
        std::string newText;
    };

    ReplaceAllCommand(Document& doc, std::vector<Replacement> replacements);
    void execute() override;
    void undo() override;
    std::string description() const override;

private:
    Document& doc_;
    std::vector<Replacement> replacements_; // stored in reverse order of application for easy undo
    bool executed_ = false;
};

#endif // COMMAND_H
