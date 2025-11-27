#pragma once
#include <string>
#include <memory>

template<typename T>
class Stack {
public:
    Stack();
    ~Stack();
    Stack(Stack&&) noexcept;
    Stack& operator=(Stack&&) noexcept;

    // Запрещаем копирование, чтобы избежать сложностей глубокого копирования
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value);
    T pop();
    T peek() const; // --- ДОБАВЛЕНО ---
    bool empty() const;

    std::string serialize() const;
    void deserialize(const std::string& str);

private:
    struct Node; // Внутренняя структура узла
    std::unique_ptr<Node> head; // Указатель на вершину стека
    size_t count;
};