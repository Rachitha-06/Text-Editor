#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "Document.h"
#include "Command.h"
#include <string>
#include <vector>
#include <utility>

/**
 * Handles search and replace operations on a Document.
 * Provides both simple substring search and optional KMP.
 */
class SearchEngine {
public:
    struct Match {
        std::size_t lineIndex;
        std::size_t position; // character offset within the line
    };

    // Find all occurrences of pattern in the document
    // caseSensitive = true for exact match, false for case-insensitive
    static std::vector<Match> findAll(const Document& doc, const std::string& pattern,
                                      bool caseSensitive = true);

    // Find first occurrence starting from a given line/position (inclusive)
    static Match findNext(const Document& doc, const std::string& pattern,
                          std::size_t startLine = 0, std::size_t startPos = 0,
                          bool caseSensitive = true);

    // Replace first occurrence; returns true if a replacement was made
    // The caller is responsible for creating the appropriate Command for undo
    static bool replaceFirst(Document& doc, const std::string& pattern,
                             const std::string& replacement, bool caseSensitive = true);

    // Build a ReplaceAllCommand for all occurrences (does not execute it)
    // Returns the command (may contain zero replacements)
    static std::unique_ptr<ReplaceAllCommand> buildReplaceAllCommand(
        Document& doc, const std::string& pattern, const std::string& replacement,
        bool caseSensitive = true);

    // Optional: KMP search for a single string (used internally or for demonstration)
    static std::vector<std::size_t> kmpSearch(const std::string& text,
                                              const std::string& pattern);

private:
    static std::string toLower(const std::string& s);
    static bool matchesAt(const std::string& text, std::size_t pos,
                          const std::string& pattern, bool caseSensitive);
    static std::vector<int> computeLPS(const std::string& pattern);
};

#endif // SEARCH_ENGINE_H
