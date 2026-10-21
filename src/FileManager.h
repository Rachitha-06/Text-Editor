#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Document.h"
#include <string>
#include <utility>

/**
 * Handles loading and saving documents from/to the filesystem.
 * Uses standard C++ file streams. UTF-8 is supported to the extent that
 * the underlying streams and terminal do; no special Unicode normalization.
 */
class FileManager {
public:
    struct Result {
        bool success;
        std::string message;
    };

    // Load file into the given document (clears existing content on success)
    static Result load(Document& doc, const std::string& path);

    // Save the document to its current filename
    static Result save(Document& doc);

    // Save the document to a new path (Save As). Updates filename only on success.
    static Result saveAs(Document& doc, const std::string& path);

    // Check whether a path looks writable / exists (best-effort)
    static bool fileExists(const std::string& path);
};

#endif // FILE_MANAGER_H
