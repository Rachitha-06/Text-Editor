#include "Command.h"
#include <stdexcept>

// ---------- InsertLineCommand ----------
InsertLineCommand::InsertLineCommand(Document& doc, std::size_t index, const std::string& text)
    : doc_(doc), index_(index), text_(text) {}

void InsertLineCommand::execute() {
    if (executed_) return;
    doc_.insertLine(index_, text_);
    executed_ = true;
}

void InsertLineCommand::undo() {
    if (!executed_) return;
    doc_.deleteLine(index_);
    executed_ = false;
}

std::string InsertLineCommand::description() const {
    return "Insert line at " + std::to_string(index_);
}

// ---------- DeleteLineCommand ----------
DeleteLineCommand::DeleteLineCommand(Document& doc, std::size_t index)
    : doc_(doc), index_(index) {}

void DeleteLineCommand::execute() {
    if (executed_) return;
    deletedText_ = doc_.getLine(index_);
    doc_.deleteLine(index_);
    executed_ = true;
}

void DeleteLineCommand::undo() {
    if (!executed_) return;
    doc_.insertLine(index_, deletedText_);
    executed_ = false;
}

std::string DeleteLineCommand::description() const {
    return "Delete line at " + std::to_string(index_);
}

// ---------- InsertTextCommand ----------
InsertTextCommand::InsertTextCommand(Document& doc, std::size_t lineIndex, std::size_t charPos,
                                     const std::string& text)
    : doc_(doc), lineIndex_(lineIndex), charPos_(charPos), text_(text) {}

void InsertTextCommand::execute() {
    if (executed_) return;
    doc_.insertText(lineIndex_, charPos_, text_);
    executed_ = true;
}

void InsertTextCommand::undo() {
    if (!executed_) return;
    doc_.deleteText(lineIndex_, charPos_, text_.size());
    executed_ = false;
}

std::string InsertTextCommand::description() const {
    return "Insert text in line " + std::to_string(lineIndex_);
}

// ---------- DeleteTextCommand ----------
DeleteTextCommand::DeleteTextCommand(Document& doc, std::size_t lineIndex, std::size_t startPos,
                                     std::size_t length)
    : doc_(doc), lineIndex_(lineIndex), startPos_(startPos), length_(length) {}

void DeleteTextCommand::execute() {
    if (executed_) return;
    const std::string& line = doc_.getLine(lineIndex_);
    if (startPos_ > line.size()) {
        throw std::out_of_range("DeleteTextCommand: start position out of range");
    }
    length_ = std::min(length_, line.size() - startPos_);
    deletedText_ = line.substr(startPos_, length_);
    doc_.deleteText(lineIndex_, startPos_, length_);
    executed_ = true;
}

void DeleteTextCommand::undo() {
    if (!executed_) return;
    doc_.insertText(lineIndex_, startPos_, deletedText_);
    executed_ = false;
}

std::string DeleteTextCommand::description() const {
    return "Delete text in line " + std::to_string(lineIndex_);
}

// ---------- ReplaceTextCommand ----------
ReplaceTextCommand::ReplaceTextCommand(Document& doc, std::size_t lineIndex, std::size_t startPos,
                                       std::size_t length, const std::string& replacement)
    : doc_(doc), lineIndex_(lineIndex), startPos_(startPos), oldLength_(length),
      newText_(replacement) {}

void ReplaceTextCommand::execute() {
    if (executed_) return;
    const std::string& line = doc_.getLine(lineIndex_);
    if (startPos_ > line.size()) {
        throw std::out_of_range("ReplaceTextCommand: start position out of range");
    }
    oldLength_ = std::min(oldLength_, line.size() - startPos_);
    oldText_ = line.substr(startPos_, oldLength_);
    doc_.replaceTextInLine(lineIndex_, startPos_, oldLength_, newText_);
    executed_ = true;
}

void ReplaceTextCommand::undo() {
    if (!executed_) return;
    // Replace the new text back with the old text
    doc_.replaceTextInLine(lineIndex_, startPos_, newText_.size(), oldText_);
    executed_ = false;
}

std::string ReplaceTextCommand::description() const {
    return "Replace text in line " + std::to_string(lineIndex_);
}

// ---------- ReplaceAllCommand ----------
ReplaceAllCommand::ReplaceAllCommand(Document& doc, std::vector<Replacement> replacements)
    : doc_(doc), replacements_(std::move(replacements)) {}

void ReplaceAllCommand::execute() {
    if (executed_) return;
    // Apply in reverse order so earlier positions remain valid
    for (auto it = replacements_.rbegin(); it != replacements_.rend(); ++it) {
        doc_.replaceTextInLine(it->lineIndex, it->startPos, it->oldLength, it->newText);
    }
    executed_ = true;
}

void ReplaceAllCommand::undo() {
    if (!executed_) return;
    // Undo in original order (forward)
    for (const auto& r : replacements_) {
        doc_.replaceTextInLine(r.lineIndex, r.startPos, r.newText.size(), r.oldText);
    }
    executed_ = false;
}

std::string ReplaceAllCommand::description() const {
    return "Replace all (" + std::to_string(replacements_.size()) + " occurrences)";
}
