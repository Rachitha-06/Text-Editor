#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>

bool FileManager::fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

FileManager::Result FileManager::load(Document& doc, const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return {false, "Cannot open file for reading: " + path};
    }

    std::ostringstream oss;
    oss << in.rdbuf();
    if (in.bad()) {
        return {false, "Error while reading file: " + path};
    }

    doc.setFromText(oss.str());
    doc.setFilename(path);
    doc.setModified(false);
    return {true, "File loaded successfully: " + path};
}

FileManager::Result FileManager::save(Document& doc) {
    const std::string& path = doc.filename();
    if (path.empty() || path == "Untitled") {
        return {false, "No filename associated with the document. Use Save As."};
    }

    std::ofstream out(path);
    if (!out) {
        return {false, "Cannot open file for writing: " + path};
    }

    out << doc.getFullText();
    if (!out) {
        return {false, "Error while writing file: " + path};
    }

    doc.setModified(false);
    return {true, "File saved successfully: " + path};
}

FileManager::Result FileManager::saveAs(Document& doc, const std::string& path) {
    if (path.empty()) {
        return {false, "Destination path cannot be empty."};
    }

    std::ofstream out(path);
    if (!out) {
        return {false, "Cannot open file for writing: " + path};
    }

    out << doc.getFullText();
    if (!out) {
        return {false, "Error while writing file: " + path};
    }

    doc.setFilename(path);
    doc.setModified(false);
    return {true, "File saved as: " + path};
}
