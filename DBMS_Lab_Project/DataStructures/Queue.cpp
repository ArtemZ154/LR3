#include "Queue.h"
#include <stdexcept>
#include <sstream>
#include "../BinaryUtils.h"

// Определение внутреннего узла
template<typename T>
struct Queue<T>::Node {
    T data;
    std::unique_ptr<Node> next;

    Node(const T& val) : data(val), next(nullptr) {}
};

// --- Конструкторы и деструктор ---
template<typename T>
Queue<T>::Queue() : head(nullptr), tail(nullptr), count(0) {}

template<typename T>
Queue<T>::~Queue() = default;

template<typename T>
Queue<T>::Queue(Queue&& other) noexcept = default;

template<typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept = default;


// --- Публичные методы ---
template<typename T>
void Queue<T>::push(const T& value) {
    auto newNode = std::make_unique<Node>(value);
    Node* newNodePtr = newNode.get();

    if (empty()) {
        head = std::move(newNode);
        tail = newNodePtr;
    } else {
        tail->next = std::move(newNode);
        tail = newNodePtr;
    }
    count++;
}

template<typename T>
T Queue<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    T value = head->data;
    head = std::move(head->next);
    count--;

    // Если после удаления очередь стала пустой, нужно сбросить и хвост
    if (empty()) {
        tail = nullptr;
    }

    return value;
}

template<typename T>
bool Queue<T>::empty() const {
    return head == nullptr;
}

template<typename T>
std::string Queue<T>::serialize() const {
    std::stringstream ss;
    writeSize(ss, count);
    Node* current = head.get();
    while (current) {
        writeValue(ss, current->data);
        current = current->next.get();
    }
    return ss.str();
}

template<typename T>
void Queue<T>::deserialize(const std::string& str) {
    head.reset();
    tail = nullptr;
    count = 0;

    if (str.empty()) return;

    std::stringstream ss(str);
    size_t size;
    readSize(ss, size);

    for(size_t i = 0; i < size; ++i) {
        T item;
        readValue(ss, item);
        push(item);
    }
}

template class Queue<std::string>;
template class Queue<int>;
