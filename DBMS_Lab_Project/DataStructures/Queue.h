#pragma once
#include <string>
#include <memory>

template<typename T>
class Queue {
public:
    Queue();
    ~Queue();
    Queue(Queue&& other) noexcept;
    Queue& operator=(Queue&& other) noexcept;
    Queue(const Queue& other) = delete;
    Queue& operator=(const Queue& other) = delete;

    void push(const T& value);
    T pop();
    bool empty() const;

    std::string serialize() const;
    void deserialize(const std::string& str);

private:
    struct Node;
    std::unique_ptr<Node> head;
    Node* tail;
    size_t count;
};