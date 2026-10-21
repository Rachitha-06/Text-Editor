# Test Cases

The automated suite (`tests/test_main.cpp`) implements the following categories.

## Doubly Linked List
- Empty list invariants
- Insert at front, middle, end
- Delete first, middle, last, only node
- Clear
- Size and at() correctness

## Document
- Append / insert / delete lines
- Insert / delete text inside a line
- Statistics (lines, words, characters)
- setFromText / getFullText round-trip
- Modified flag

## Undo / Redo
- Undo insertion and deletion
- Redo after undo
- Multiple undo steps
- New edit after undo clears redo stack

## Search & Replace
- Find single and multiple occurrences
- Case-sensitive vs case-insensitive
- Replace first
- Replace all
- Empty pattern returns zero matches
- KMP on repeated and overlapping patterns

## File Manager
- Load existing file
- Save existing document
- Save As to a new path
- Missing file returns error without crashing
- Modified flag cleared only after successful save

## Edge Cases
- Empty document
- Out-of-range line / character indices are rejected
- Empty search pattern

Manual testing additionally covers the full interactive menu, confirmation dialogs, and pagination of long documents.
