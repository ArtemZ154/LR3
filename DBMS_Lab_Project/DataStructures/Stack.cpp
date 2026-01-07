#include "Stack.h"
#include <sstream>
#include "../BinaryUtils.h"
#include <stdexcept>
#include <vector>
#include <algorithm> // для std::reverse

// Определение внутренней структуры Node
template<typename T>
struct Stack<T>::Node {
    T data;
    std::unique_ptr<Node> next;
    Node(T val) : data(val), next(nullptr) {}
};

// --- Конструкторы и деструктор ---
template<typename T>
Stack<T>::Stack() : head(nullptr), count(0) {}

template<typename T>
Stack<T>::~Stack() {
    // unique_ptr automatically cleans up the chain
    while(head) {
        head = std::move(head->next);
    }
}

// --- Реализация семантики перемещения ---
template<typename T>
Stack<T>::Stack(Stack&& other) noexcept
    : head(std::move(other.head)), count(other.count) {
    other.count = 0;
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
    if (this != &other) {
        head = std::move(other.head);
        count = other.count;
        other.count = 0;
    }
    return *this;
}

// --- Основные методы стека ---
template<typename T>
void Stack<T>::push(const T& value) {
    auto newNode = std::make_unique<Node>(value);
    newNode->next = std::move(head);
    head = std::move(newNode);
    count++;
}

template<typename T>
T Stack<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Stack is empty");
    }
    T value = head->data;
    head = std::move(head->next);
    count--;
    return value;
}

// --- ДОБАВЛЕНА РЕАЛИЗАЦИЯ PEEK ---
template<typename T>
T Stack<T>::peek() const {
    if (empty()) {
        throw std::runtime_error("Stack is empty");
    }
    return head->data;
}
// --- КОНЕЦ ---

template<typename T>
bool Stack<T>::empty() const {
    return head == nullptr;
}

// --- Сериализация и десериализация ---
template<typename T>
std::string Stack<T>::serialize() const {
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
void Stack<T>::deserialize(const std::string& str) {
    head.reset();
    count = 0;
    if (str.empty()) return;

    Stack<T> temp_stack;
    std::stringstream ss(str);
    size_t size;
    readSize(ss, size);

    for (size_t i = 0; i < size; ++i) {
        T item;
        readValue(ss, item);
        temp_stack.push(item);
    }

    while (!temp_stack.empty()) {
        this->push(temp_stack.pop());
    }
}

// --- Явное инстанцирование ---
template class Stack<std::string>;

// --- ЭТА СТРОКА ИСПРАВЛЯЕТ ТВОЮ ОШИБКУ ---
template class Stack<int>;