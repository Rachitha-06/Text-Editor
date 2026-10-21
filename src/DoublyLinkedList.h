#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <string>
#include <stdexcept>
#include <cstddef>

/**
 * Custom doubly linked list where each node holds one line of text.
 * Used as the primary document representation to demonstrate linked-list concepts.
 * Ownership: Document owns the list; nodes are managed with raw pointers under RAII
 * (Document destructor and clear() free all nodes).
 */
class DoublyLinkedList {
public:
    struct Node {
        std::string text;
        Node* prev;
        Node* next;

        explicit Node(const std::string& t = "")
            : text(t), prev(nullptr), next(nullptr) {}
    };

    DoublyLinkedList();
    ~DoublyLinkedList();

    // Disable copy for simplicity and safety (deep copy can be added later)
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    // Move support
    DoublyLinkedList(DoublyLinkedList&& other) noexcept;
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;

    void clear();
    bool empty() const;
    std::size_t size() const;

    // Insert a new line at the given 0-based index. Index == size() appends.
    void insert(std::size_t index, const std::string& text);

    // Delete the line at the given 0-based index.
    void remove(std::size_t index);

    // Access line text (const and non-const)
    const std::string& at(std::size_t index) const;
    std::string& at(std::size_t index);

    // Get node pointer (for advanced use / undo); returns nullptr if out of range
    Node* getNode(std::size_t index);
    const Node* getNode(std::size_t index) const;

    Node* head() { return head_; }
    const Node* head() const { return head_; }
    Node* tail() { return tail_; }
    const Node* tail() const { return tail_; }

private:
    Node* head_;
    Node* tail_;
    std::size_t size_;

    Node* findNode(std::size_t index) const;
};

#endif // DOUBLY_LINKED_LIST_H
