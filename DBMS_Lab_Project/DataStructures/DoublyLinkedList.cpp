#include "DoublyLinkedList.h"
#include <sstream>
#include "../BinaryUtils.h"
#include <stdexcept>
template<typename T> struct DoublyLinkedList<T>::Node {
    T data;
    std::unique_ptr<Node> next;
    Node* prev;
    Node(T val) : data(val), next(nullptr), prev(nullptr) {}
};
template<typename T> DoublyLinkedList<T>::DoublyLinkedList() = default;
template<typename T> DoublyLinkedList<T>::~DoublyLinkedList() = default;
template<typename T> DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&&) noexcept = default;
template<typename T> DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList&&) noexcept = default;
template<typename T> void DoublyLinkedList<T>::push_front(const T& value) {
    auto newNode = std::make_unique<Node>(value);
    if (head) {
        head->prev = newNode.get();
        newNode->next = std::move(head);
    } else { tail = newNode.get(); }
    head = std::move(newNode);
    count++;
}

template<typename T> bool DoublyLinkedList<T>::find(const T& value) const {
    for (Node* curr = head.get(); curr; curr = curr->next.get()) {
        if (curr->data == value) return true;
    }
    return false;
}

template<typename T> void DoublyLinkedList<T>::remove_value(const T& value) {
    Node* current = head.get();
    while (current && current->data != value) {
        current = current->next.get();
    }
    if (!current) return; // Не найдено
    if (current->prev) current->prev->next = std::move(current->next);
    else head = std::move(current->next); // Это был head
    if (current->next) current->next->prev = current->prev;
    else tail = current->prev; // Это был tail
    count--;
}
template<typename T> void DoublyLinkedList<T>::push_back(const T& value) {
    auto newNode = std::make_unique<Node>(value);
    if (tail) {
        newNode->prev = tail;
        tail->next = std::move(newNode);
        tail = tail->next.get();
    } else {
        head = std::move(newNode);
        tail = head.get();
    }
    count++;
}
template<typename T> T DoublyLinkedList<T>::pop_front() {
    if (!head) throw std::runtime_error("List is empty");
    T val = head->data;
    head = std::move(head->next);
    if (head) head->prev = nullptr; else tail = nullptr;
    count--;
    return val;
}
template<typename T> T DoublyLinkedList<T>::pop_back() {
    if (!tail) throw std::runtime_error("List is empty");
    T val = tail->data;
    if (tail->prev) {
        tail = tail->prev;
        tail->next.reset();
    } else { head.reset(); tail = nullptr; }
    count--;
    return val;
}
template<typename T> size_t DoublyLinkedList<T>::size() const { return count; }
template<typename T> std::string DoublyLinkedList<T>::serialize() const {
    std::stringstream ss;
    writeSize(ss, count);
    for (Node* curr = head.get(); curr; curr = curr->next.get()) {
        writeValue(ss, curr->data);
    }
    return ss.str();
}
template<typename T> void DoublyLinkedList<T>::deserialize(const std::string& str) {
    head.reset(); tail = nullptr; count = 0;
    if (str.empty()) return;
    std::stringstream ss(str);
    size_t size;
    readSize(ss, size);
    for(size_t i = 0; i < size; ++i) {
        T item;
        readValue(ss, item);
        push_back(item);
    }
}
template<typename T>
void DoublyLinkedList<T>::insert_after(const T& target_value, const T& new_value) {
    Node* current = head.get();
    while (current && current->data != target_value) {
        current = current->next.get();
    }
    if (!current) return; // Не найдено

    auto newNode = std::make_unique<Node>(new_value);
    newNode->prev = current;
    if (current->next) {
        newNode->next = std::move(current->next);
        newNode->next->prev = newNode.get();
    } else {
        tail = newNode.get(); // Вставка после хвоста
    }
    current->next = std::move(newNode);
    count++;
}

template<typename T>
void DoublyLinkedList<T>::remove_after(const T& target_value) {
    Node* current = head.get();
    while (current && current->data != target_value) {
        current = current->next.get();
    }

    if (!current || !current->next) return;

    Node* node_to_delete = current->next.get();

    current->next = std::move(node_to_delete->next);
    if (current->next) {
        current->next->prev = current;
    } else {
        tail = current;
    }
    count--;
}

template<typename T>
void DoublyLinkedList<T>::remove_before(const T& target_value) {
    Node* current = head.get();
    while (current && current->data != target_value) {
        current = current->next.get();
    }

    if (!current || !current->prev) return;

    Node* node_to_delete = current->prev;

    if (node_to_delete->prev) {
        node_to_delete->prev->next = std::move(current->prev->next);
        current->prev = node_to_delete->prev;
    } else {
        head = std::move(current->prev->next);
        current->prev = nullptr;
    }
    count--;
}

template<typename T>
void DoublyLinkedList<T>::insert_before(const T& target_value, const T& new_value) {
    Node* current = head.get();
    while (current && current->data != target_value) {
        current = current->next.get();
    }
    if (!current) return; // Не найдено

    if (current == head.get()) {
        push_front(new_value);
        return;
    }

    auto newNode = std::make_unique<Node>(new_value);
    newNode->next = std::move(current->prev->next);
    newNode->prev = current->prev;
    current->prev->next = std::move(newNode);
    current->prev = newNode.get();
    count++;
}

template class DoublyLinkedList<std::string>;

