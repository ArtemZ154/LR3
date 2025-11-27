#include "Stack.h"
#include <sstream>
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
    Node* current = head.get();
    while (current) {
        ss << current->data << (current->next ? " " : "");
        current = current->next.get();
    }
    return ss.str();
}

template<typename T>
void Stack<T>::deserialize(const std::string& str) {
    head.reset(); // Clear the current stack
    count = 0;
    if (str.empty()) return;

    Stack<T> temp_stack;
    std::stringstream ss(str);
    T item;

    // Read all items into a temporary stack
    while (ss >> item) {
        temp_stack.push(item);
    }

    // Move from temp_stack to this, reversing the order
    // to match the original stack structure
    while (!temp_stack.empty()) {
        this->push(temp_stack.pop());
    }
}

// --- Явное инстанцирование ---
template class Stack<std::string>;

// --- ЭТА СТРОКА ИСПРАВЛЯЕТ ТВОЮ ОШИБКУ ---
template class Stack<int>;