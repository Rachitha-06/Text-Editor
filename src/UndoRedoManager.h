#ifndef UNDO_REDO_MANAGER_H
#define UNDO_REDO_MANAGER_H

#include "Command.h"
#include <memory>
#include <stack>
#include <string>

/**
 * Manages undo and redo stacks of commands.
 * New commands push onto the undo stack and clear the redo stack.
 */
class UndoRedoManager {
public:
    UndoRedoManager() = default;

    // Execute a new command and record it for undo
    void executeCommand(std::unique_ptr<Command> cmd);

    // Undo the last command (if any)
    bool undo();

    // Redo the last undone command (if any)
    bool redo();

    bool canUndo() const;
    bool canRedo() const;

    void clear();

    std::string lastUndoDescription() const;
    std::string lastRedoDescription() const;

private:
    std::stack<std::unique_ptr<Command>> undoStack_;
    std::stack<std::unique_ptr<Command>> redoStack_;
};

#endif // UNDO_REDO_MANAGER_H
