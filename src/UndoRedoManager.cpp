#include "UndoRedoManager.h"

void UndoRedoManager::executeCommand(std::unique_ptr<Command> cmd) {
    if (!cmd) return;
    cmd->execute();
    undoStack_.push(std::move(cmd));
    // Clear redo history on new edit
    while (!redoStack_.empty()) {
        redoStack_.pop();
    }
}

bool UndoRedoManager::undo() {
    if (undoStack_.empty()) return false;
    auto cmd = std::move(undoStack_.top());
    undoStack_.pop();
    cmd->undo();
    redoStack_.push(std::move(cmd));
    return true;
}

bool UndoRedoManager::redo() {
    if (redoStack_.empty()) return false;
    auto cmd = std::move(redoStack_.top());
    redoStack_.pop();
    cmd->execute();
    undoStack_.push(std::move(cmd));
    return true;
}

bool UndoRedoManager::canUndo() const {
    return !undoStack_.empty();
}

bool UndoRedoManager::canRedo() const {
    return !redoStack_.empty();
}

void UndoRedoManager::clear() {
    while (!undoStack_.empty()) undoStack_.pop();
    while (!redoStack_.empty()) redoStack_.pop();
}

std::string UndoRedoManager::lastUndoDescription() const {
    if (undoStack_.empty()) return "";
    return undoStack_.top()->description();
}

std::string UndoRedoManager::lastRedoDescription() const {
    if (redoStack_.empty()) return "";
    return redoStack_.top()->description();
}
