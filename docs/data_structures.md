# Data Structures Used in LineForge

## 1. Custom Doubly Linked List (Primary Document Representation)

### Why a doubly linked list?

Each node stores one **line** of text. This design:

- Makes insertion and deletion of lines O(1) once the node is located (plus the cost of finding the node).
- Naturally supports efficient navigation forward and backward.
- Demonstrates classic linked-list concepts required by many academic curricula.
- Avoids the shifting cost that a `std::vector<std::string>` would incur on every middle insertion/deletion of a line.

### Node structure

```cpp
struct Node {
    std::string text;
    Node* prev;
    Node* next;
};
```

### List invariants

- `head_` points to the first line (or `nullptr` if empty).
- `tail_` points to the last line (or `nullptr` if empty).
- `size_` is the exact number of nodes.
- Every node’s `prev`/`next` links are consistent; the list is never circular.

### Memory ownership

The `DoublyLinkedList` (and therefore the `Document`) **owns** all nodes.  
Raw pointers are used for educational clarity; the destructor and `clear()` walk the list and `delete` every node, preventing leaks. Move semantics are supported; copy is disabled to avoid accidental deep copies.

### Complexity summary (see also complexity_analysis.md)

| Operation              | Time (average) | Notes                                      |
|------------------------|----------------|--------------------------------------------|
| Append line            | O(1)           | Uses tail pointer                          |
| Insert/delete at known node | O(1)      |                                            |
| Access / insert / delete by index | O(n) | Linear search from nearer end              |
| Clear                  | O(n)           |                                            |

## 2. Stacks for Undo / Redo (Command Pattern)

Two `std::stack<std::unique_ptr<Command>>` objects are maintained:

- **Undo stack** – commands that have been executed.
- **Redo stack** – commands that have been undone.

When a new editing command is executed:

1. It is pushed onto the undo stack.
2. The redo stack is cleared (standard editor behaviour).

Undo moves a command from undo → redo (calling `undo()`).  
Redo moves a command from redo → undo (calling `execute()` again).

### Why the Command pattern?

- Each concrete command (`InsertLineCommand`, `DeleteTextCommand`, `ReplaceAllCommand`, …) stores exactly the information needed to reverse itself.
- Memory usage grows only with the number of operations, not with document size (unlike full snapshots).
- Easy to extend with new editable operations.

Alternative considered: full document snapshots on every edit. Simpler to implement but far more memory-hungry and slower for large documents.

## 3. Supporting Standard Library Structures

- `std::string` – line content and pattern matching.
- `std::vector` – temporary storage of search matches and Replace-All records.
- `std::unique_ptr` – exclusive ownership of commands and the current document.

## Advantages & Trade-offs

| Approach                    | Pros                              | Cons                                      |
|-----------------------------|-----------------------------------|-------------------------------------------|
| Doubly linked list of lines | Educational, O(1) line splice     | O(n) random access by line number         |
| `std::vector<std::string>`  | Excellent locality, O(1) access   | Line insert/delete in middle is O(n)      |
| Rope / Gap buffer           | Excellent for character edits     | Significantly more complex implementation |

For a student project that must demonstrate linked lists, the doubly-linked line list is an appropriate and clear choice.
