#include "Document.h"
#include <sstream>
#include <cctype>
#include <algorithm>

Document::Document()
    : filename_("Untitled"), modified_(false) {}

Document::Document(const std::string& filename)
    : filename_(filename), modified_(false) {}

bool Document::empty() const {
    return lines_.empty();
}

std::size_t Document::lineCount() const {
    return lines_.size();
}

const std::string& Document::filename() const {
    return filename_;
}

void Document::setFilename(const std::string& name) {
    filename_ = name;
}

bool Document::isModified() const {
    return modified_;
}

void Document::setModified(bool modified) {
    modified_ = modified;
}

void Document::insertLine(std::size_t index, const std::string& text) {
    lines_.insert(index, text);
    modified_ = true;
}

void Document::appendLine(const std::string& text) {
    lines_.insert(lines_.size(), text);
    modified_ = true;
}

void Document::deleteLine(std::size_t index) {
    lines_.remove(index);
    modified_ = true;
}

void Document::replaceLine(std::size_t index, const std::string& text) {
    lines_.at(index) = text;
    modified_ = true;
}

const std::string& Document::getLine(std::size_t index) const {
    return lines_.at(index);
}

std::string& Document::getLine(std::size_t index) {
    return lines_.at(index);
}

void Document::insertText(std::size_t lineIndex, std::size_t charPos, const std::string& text) {
    std::string& line = lines_.at(lineIndex);
    if (charPos > line.size()) {
        throw std::out_of_range("Document::insertText: character position out of range");
    }
    line.insert(charPos, text);
    modified_ = true;
}

void Document::deleteText(std::size_t lineIndex, std::size_t startPos, std::size_t length) {
    std::string& line = lines_.at(lineIndex);
    if (startPos > line.size()) {
        throw std::out_of_range("Document::deleteText: start position out of range");
    }
    if (startPos + length > line.size()) {
        length = line.size() - startPos;
    }
    line.erase(startPos, length);
    modified_ = true;
}

void Document::replaceTextInLine(std::size_t lineIndex, std::size_t startPos, std::size_t length,
                                 const std::string& replacement) {
    std::string& line = lines_.at(lineIndex);
    if (startPos > line.size()) {
        throw std::out_of_range("Document::replaceTextInLine: start position out of range");
    }
    if (startPos + length > line.size()) {
        length = line.size() - startPos;
    }
    line.replace(startPos, length, replacement);
    modified_ = true;
}

void Document::clear() {
    lines_.clear();
    modified_ = true;
}

std::string Document::getFullText() const {
    if (lines_.empty()) return "";
    std::ostringstream oss;
    auto* node = lines_.head();
    bool first = true;
    while (node) {
        if (!first) oss << '\n';
        oss << node->text;
        first = false;
        node = node->next;
    }
    return oss.str();
}

void Document::setFromText(const std::string& text) {
    lines_.clear();
    if (text.empty()) {
        modified_ = true;
        return;
    }
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line)) {
        lines_.insert(lines_.size(), line);
    }
    // Handle trailing newline: if text ends with \n, getline may not add empty last line
    // depending on implementation; we keep simple behavior.
    modified_ = true;
}

std::size_t Document::countWords(const std::string& s) {
    std::size_t count = 0;
    bool inWord = false;
    for (char c : s) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            ++count;
        }
    }
    return count;
}

Document::Stats Document::calculateStats() const {
    Stats s;
    s.lines = lines_.size();
    std::size_t totalLen = 0;
    auto* node = lines_.head();
    while (node) {
        const std::string& t = node->text;
        totalLen += t.size();
        s.characters += t.size();
        s.words += countWords(t);
        if (t.empty()) ++s.emptyLines;
        if (t.size() > s.longestLineLength) {
            s.longestLineLength = t.size();
        }
        node = node->next;
    }
    s.charactersWithNewlines = s.characters + (s.lines > 0 ? s.lines - 1 : 0);
    s.averageLineLength = s.lines > 0 ? static_cast<double>(totalLen) / s.lines : 0.0;
    return s;
}
