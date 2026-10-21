#include "DoublyLinkedList.h"
#include <utility>

DoublyLinkedList::DoublyLinkedList()
    : head_(nullptr), tail_(nullptr), size_(0) {}

DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

DoublyLinkedList::DoublyLinkedList(DoublyLinkedList&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

DoublyLinkedList& DoublyLinkedList::operator=(DoublyLinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

void DoublyLinkedList::clear() {
    Node* current = head_;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
}

bool DoublyLinkedList::empty() const {
    return size_ == 0;
}

std::size_t DoublyLinkedList::size() const {
    return size_;
}

DoublyLinkedList::Node* DoublyLinkedList::findNode(std::size_t index) const {
    if (index >= size_) return nullptr;

    // Optimize: start from head or tail depending on which is closer
    if (index < size_ / 2) {
        Node* node = head_;
        for (std::size_t i = 0; i < index; ++i) {
            node = node->next;
        }
        return node;
    } else {
        Node* node = tail_;
        for (std::size_t i = size_ - 1; i > index; --i) {
            node = node->prev;
        }
        return node;
    }
}

void DoublyLinkedList::insert(std::size_t index, const std::string& text) {
    if (index > size_) {
        throw std::out_of_range("DoublyLinkedList::insert: index out of range");
    }

    Node* newNode = new Node(text);

    if (size_ == 0) {
        // Empty list
        head_ = tail_ = newNode;
    } else if (index == 0) {
        // Insert at front
        newNode->next = head_;
        head_->prev = newNode;
        head_ = newNode;
    } else if (index == size_) {
        // Append at end
        newNode->prev = tail_;
        tail_->next = newNode;
        tail_ = newNode;
    } else {
        // Insert in middle
        Node* nextNode = findNode(index);
        Node* prevNode = nextNode->prev;
        newNode->prev = prevNode;
        newNode->next = nextNode;
        prevNode->next = newNode;
        nextNode->prev = newNode;
    }
    ++size_;
}

void DoublyLinkedList::remove(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("DoublyLinkedList::remove: index out of range");
    }

    Node* node = findNode(index);

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head_ = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail_ = node->prev;
    }

    delete node;
    --size_;
}

const std::string& DoublyLinkedList::at(std::size_t index) const {
    Node* node = findNode(index);
    if (!node) {
        throw std::out_of_range("DoublyLinkedList::at: index out of range");
    }
    return node->text;
}

std::string& DoublyLinkedList::at(std::size_t index) {
    Node* node = findNode(index);
    if (!node) {
        throw std::out_of_range("DoublyLinkedList::at: index out of range");
    }
    return node->text;
}

DoublyLinkedList::Node* DoublyLinkedList::getNode(std::size_t index) {
    return findNode(index);
}

const DoublyLinkedList::Node* DoublyLinkedList::getNode(std::size_t index) const {
    return findNode(index);
}
