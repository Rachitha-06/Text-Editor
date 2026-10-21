#include "Editor.h"
#include <stdexcept>

Editor::Editor() : doc_(nullptr) {}

Document& Editor::document() {
    if (!doc_) throw std::runtime_error("No document is open");
    return *doc_;
}

const Document& Editor::document() const {
    if (!doc_) throw std::runtime_error("No document is open");
    return *doc_;
}

bool Editor::hasDocument() const {
    return doc_ != nullptr;
}

void Editor::newDocument() {
    doc_ = std::make_unique<Document>();
    history_.clear();
}

void Editor::closeDocument() {
    doc_.reset();
    history_.clear();
}

FileManager::Result Editor::openFile(const std::string& path) {
    auto newDoc = std::make_unique<Document>();
    auto result = FileManager::load(*newDoc, path);
    if (result.success) {
        doc_ = std::move(newDoc);
        history_.clear();
    }
    return result;
}

FileManager::Result Editor::saveFile() {
    if (!doc_) return {false, "No document is open"};
    return FileManager::save(*doc_);
}

FileManager::Result Editor::saveFileAs(const std::string& path) {
    if (!doc_) return {false, "No document is open"};
    return FileManager::saveAs(*doc_, path);
}

bool Editor::insertLine(std::size_t index, const std::string& text) {
    if (!doc_) return false;
    if (index > doc_->lineCount()) return false;
    auto cmd = std::make_unique<InsertLineCommand>(*doc_, index, text);
    history_.executeCommand(std::move(cmd));
    return true;
}

bool Editor::appendLine(const std::string& text) {
    if (!doc_) return false;
    return insertLine(doc_->lineCount(), text);
}

bool Editor::deleteLine(std::size_t index) {
    if (!doc_) return false;
    if (index >= doc_->lineCount()) return false;
    auto cmd = std::make_unique<DeleteLineCommand>(*doc_, index);
    history_.executeCommand(std::move(cmd));
    return true;
}

bool Editor::insertText(std::size_t lineIndex, std::size_t charPos, const std::string& text) {
    if (!doc_) return false;
    if (lineIndex >= doc_->lineCount()) return false;
    try {
        auto cmd = std::make_unique<InsertTextCommand>(*doc_, lineIndex, charPos, text);
        history_.executeCommand(std::move(cmd));
        return true;
    } catch (const std::out_of_range&) {
        return false;
    }
}

bool Editor::deleteText(std::size_t lineIndex, std::size_t startPos, std::size_t length) {
    if (!doc_) return false;
    if (lineIndex >= doc_->lineCount()) return false;
    try {
        auto cmd = std::make_unique<DeleteTextCommand>(*doc_, lineIndex, startPos, length);
        history_.executeCommand(std::move(cmd));
        return true;
    } catch (const std::out_of_range&) {
        return false;
    }
}

std::vector<SearchEngine::Match> Editor::findAll(const std::string& pattern, bool caseSensitive) {
    if (!doc_) return {};
    return SearchEngine::findAll(*doc_, pattern, caseSensitive);
}

bool Editor::replaceFirst(const std::string& pattern, const std::string& replacement,
                          bool caseSensitive) {
    if (!doc_ || pattern.empty()) return false;
    auto matches = SearchEngine::findAll(*doc_, pattern, caseSensitive);
    if (matches.empty()) return false;
    const auto& m = matches.front();
    auto cmd = std::make_unique<ReplaceTextCommand>(*doc_, m.lineIndex, m.position,
                                                    pattern.size(), replacement);
    history_.executeCommand(std::move(cmd));
    return true;
}

std::size_t Editor::replaceAll(const std::string& pattern, const std::string& replacement,
                               bool caseSensitive) {
    if (!doc_ || pattern.empty()) return 0;
    auto cmd = SearchEngine::buildReplaceAllCommand(*doc_, pattern, replacement, caseSensitive);
    std::size_t count = 0;
    // Count is known after building
    // We need to inspect the command; for simplicity re-find
    auto matches = SearchEngine::findAll(*doc_, pattern, caseSensitive);
    count = matches.size();
    if (count > 0) {
        history_.executeCommand(std::move(cmd));
    }
    return count;
}

bool Editor::undo() {
    return history_.undo();
}

bool Editor::redo() {
    return history_.redo();
}

bool Editor::canUndo() const {
    return history_.canUndo();
}

bool Editor::canRedo() const {
    return history_.canRedo();
}

std::string Editor::undoDescription() const {
    return history_.lastUndoDescription();
}

std::string Editor::redoDescription() const {
    return history_.lastRedoDescription();
}

Document::Stats Editor::stats() const {
    if (!doc_) return {};
    return doc_->calculateStats();
}
