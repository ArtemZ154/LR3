#include "SinglyLinkedList.h"
#include "../BinaryUtils.h"
#include <sstream>
#include <stdexcept>
template<typename T> struct SinglyLinkedList<T>::Node {
    T data;
    std::unique_ptr<Node> next;
    Node(T val) : data(val), next(nullptr) {}
};
template<typename T> SinglyLinkedList<T>::SinglyLinkedList() = default;
template<typename T> SinglyLinkedList<T>::~SinglyLinkedList() = default;
template<typename T> SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList&&) noexcept = default;
template<typename T> SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList&&) noexcept = default;

template<typename T> void SinglyLinkedList<T>::push_front(const T& value) {
    auto newNode = std::make_unique<Node>(value);
    if (head) { newNode->next = std::move(head); }
    else { tail = newNode.get(); }
    head = std::move(newNode);
    count++;
}

template<typename T> bool SinglyLinkedList<T>::find(const T& value) const {
    for (Node* curr = head.get(); curr; curr = curr->next.get()) {
        if (curr->data == value) return true;
    }
    return false;
}

template<typename T> void SinglyLinkedList<T>::remove_value(const T& value) {
    if (!head) return;
    if (head->data == value) {
        pop_front();
        return;
    }
    Node* current = head.get();
    while (current->next && current->next->data != value) {
        current = current->next.get();
    }
    if (current->next) {
        if (tail == current->next.get()) tail = current;
        current->next = std::move(current->next->next);
        count--;
    }
}
template<typename T> void SinglyLinkedList<T>::push_back(const T& value) {
    auto newNode = std::make_unique<Node>(value);
    if (tail) {
        tail->next = std::move(newNode);
        tail = tail->next.get();
    } else {
        head = std::move(newNode);
        tail = head.get();
    }
    count++;
}
template<typename T> T SinglyLinkedList<T>::pop_front() {
    if (!head) throw std::runtime_error("List is empty");
    T val = head->data;
    if (tail == head.get()) tail = nullptr;
    head = std::move(head->next);
    count--;
    return val;
}

template<typename T> T SinglyLinkedList<T>::pop_back() {
    if (!head) throw std::runtime_error("List is empty");
    T val = tail->data;
    if (head.get() == tail) {
        head.reset();
        tail = nullptr;
    } else {
        Node* current = head.get();
        while (current->next.get() != tail) {
            current = current->next.get();
        }
        tail = current;
        tail->next.reset();
    }
    count--;
    return val;
}

template<typename T> std::string SinglyLinkedList<T>::serialize() const {
    std::stringstream ss;
    writeSize(ss, count);
    for (Node* curr = head.get(); curr; curr = curr->next.get()) {
        writeValue(ss, curr->data);
    }
    return ss.str();
}
template<typename T> void SinglyLinkedList<T>::deserialize(const std::string& str) {
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
void SinglyLinkedList<T>::insert_after(const T& target_value, const T& new_value) {
    Node* current = head.get();
    while (current && current->data != target_value) {
        current = current->next.get();
    }

    if (!current) return; // Элемент не найден

    auto newNode = std::make_unique<Node>(new_value);
    newNode->next = std::move(current->next);
    current->next = std::move(newNode);
    if (tail == current) { // Если вставляли после хвоста
        tail = current->next.get();
    }
    count++;
}

template<typename T>
void SinglyLinkedList<T>::insert_before(const T& target_value, const T& new_value) {
    if (!head) return;

    if (head->data == target_value) {
        push_front(new_value);
        return;
    }

    Node* prev = head.get();
    while (prev->next && prev->next->data != target_value) {
        prev = prev->next.get();
    }

    if (!prev->next) return; // Элемент не найден

    auto newNode = std::make_unique<Node>(new_value);
    newNode->next = std::move(prev->next);
    prev->next = std::move(newNode);
    count++;
}

template<typename T>
void SinglyLinkedList<T>::remove_after(const T& target_value) {
    Node* current = head.get();
    while (current && current->data != target_value) {
        current = current->next.get();
    }

    if (!current || !current->next) return;
    if (tail == current->next.get()) {
        tail = current;
    }

    current->next = std::move(current->next->next);
    count--;
}

template<typename T>
void SinglyLinkedList<T>::remove_before(const T& target_value) {
    if (!head || !head->next) return;

    if (head->next->data == target_value) {
        pop_front();
        return;
    }

    Node* prev_prev = head.get();
    while (prev_prev->next && prev_prev->next->next) {
        if (prev_prev->next->next->data == target_value) {
            prev_prev->next = std::move(prev_prev->next->next);
            count--;
            return;
        }
        prev_prev = prev_prev->next.get();
    }
}


template class SinglyLinkedList<std::string>;