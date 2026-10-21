#include "SearchEngine.h"
#include <algorithm>
#include <cctype>

std::string SearchEngine::toLower(const std::string& s) {
    std::string result = s;
    for (char& c : result) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

bool SearchEngine::matchesAt(const std::string& text, std::size_t pos,
                             const std::string& pattern, bool caseSensitive) {
    if (pos + pattern.size() > text.size()) return false;
    if (caseSensitive) {
        return text.compare(pos, pattern.size(), pattern) == 0;
    }
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        char a = static_cast<char>(std::tolower(static_cast<unsigned char>(text[pos + i])));
        char b = static_cast<char>(std::tolower(static_cast<unsigned char>(pattern[i])));
        if (a != b) return false;
    }
    return true;
}

std::vector<SearchEngine::Match> SearchEngine::findAll(const Document& doc,
                                                       const std::string& pattern,
                                                       bool caseSensitive) {
    std::vector<Match> results;
    if (pattern.empty()) return results;

    const std::size_t n = doc.lineCount();
    for (std::size_t i = 0; i < n; ++i) {
        const std::string& line = doc.getLine(i);
        for (std::size_t pos = 0; pos + pattern.size() <= line.size(); ++pos) {
            if (matchesAt(line, pos, pattern, caseSensitive)) {
                results.push_back({i, pos});
                // Advance past this match to avoid overlapping reports for simple search
                // (for overlapping support one can remove the + pattern.size()-1)
                pos += pattern.size() - 1;
            }
        }
    }
    return results;
}

SearchEngine::Match SearchEngine::findNext(const Document& doc, const std::string& pattern,
                                           std::size_t startLine, std::size_t startPos,
                                           bool caseSensitive) {
    Match none{static_cast<std::size_t>(-1), static_cast<std::size_t>(-1)};
    if (pattern.empty()) return none;

    const std::size_t n = doc.lineCount();
    for (std::size_t i = startLine; i < n; ++i) {
        const std::string& line = doc.getLine(i);
        std::size_t pos = (i == startLine) ? startPos : 0;
        for (; pos + pattern.size() <= line.size(); ++pos) {
            if (matchesAt(line, pos, pattern, caseSensitive)) {
                return {i, pos};
            }
        }
    }
    return none;
}

bool SearchEngine::replaceFirst(Document& doc, const std::string& pattern,
                                const std::string& replacement, bool caseSensitive) {
    Match m = findNext(doc, pattern, 0, 0, caseSensitive);
    if (m.lineIndex == static_cast<std::size_t>(-1)) return false;
    doc.replaceTextInLine(m.lineIndex, m.position, pattern.size(), replacement);
    return true;
}

std::unique_ptr<ReplaceAllCommand> SearchEngine::buildReplaceAllCommand(
    Document& doc, const std::string& pattern, const std::string& replacement,
    bool caseSensitive) {

    std::vector<ReplaceAllCommand::Replacement> reps;
    if (pattern.empty()) {
        return std::make_unique<ReplaceAllCommand>(doc, std::move(reps));
    }

    // Collect all matches first (positions are relative to original text)
    auto matches = findAll(doc, pattern, caseSensitive);

    // Important: when replacement contains the pattern, we still use original positions
    // because we apply replacements from the end of the document forward.
    for (const auto& m : matches) {
        ReplaceAllCommand::Replacement r;
        r.lineIndex = m.lineIndex;
        r.startPos = m.position;
        r.oldLength = pattern.size();
        r.oldText = doc.getLine(m.lineIndex).substr(m.position, pattern.size());
        r.newText = replacement;
        reps.push_back(r);
    }

    return std::make_unique<ReplaceAllCommand>(doc, std::move(reps));
}

// ---------- KMP implementation ----------
std::vector<int> SearchEngine::computeLPS(const std::string& pattern) {
    std::size_t m = pattern.size();
    std::vector<int> lps(m, 0);
    int len = 0;
    std::size_t i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            ++len;
            lps[i] = len;
            ++i;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                ++i;
            }
        }
    }
    return lps;
}

std::vector<std::size_t> SearchEngine::kmpSearch(const std::string& text,
                                                 const std::string& pattern) {
    std::vector<std::size_t> positions;
    if (pattern.empty() || text.empty()) return positions;

    auto lps = computeLPS(pattern);
    std::size_t i = 0; // index for text
    std::size_t j = 0; // index for pattern
    while (i < text.size()) {
        if (pattern[j] == text[i]) {
            ++i;
            ++j;
        }
        if (j == pattern.size()) {
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < text.size() && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }
    return positions;
}
