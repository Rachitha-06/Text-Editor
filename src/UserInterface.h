#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Editor.h"
#include <string>

/**
 * Interactive console-based user interface for LineForge.
 * Handles menus, input validation, and presentation.
 */
class UserInterface {
public:
    UserInterface();
    void run();

private:
    Editor editor_;

    // Input helpers
    std::string readLine(const std::string& prompt);
    int readInt(const std::string& prompt, int minVal, int maxVal);
    bool confirm(const std::string& question);
    void pause();
    void clearScreen();

    // Main loops
    void mainMenu();
    void documentMenu();

    // Feature handlers
    void handleNewDocument();
    void handleOpenDocument();
    void handleViewDocument();
    void handleInsertText();
    void handleDeleteText();
    void handleFindText();
    void handleReplaceText();
    void handleUndo();
    void handleRedo();
    void handleSave();
    void handleSaveAs();
    void handleStatistics();
    void handleCloseDocument();

    // Display helpers
    void displayDocument(std::size_t startLine = 0, std::size_t maxLines = 20);
    void printHeader();
    void printDocumentStatus();
};

#endif // USER_INTERFACE_H
