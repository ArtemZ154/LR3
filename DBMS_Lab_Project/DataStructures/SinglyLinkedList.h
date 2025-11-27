#pragma once
#include <string>
#include <memory>

template<typename T>
class SinglyLinkedList {
private:
    struct Node;
    std::unique_ptr<Node> head;
    Node* tail = nullptr;
    size_t count = 0;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();
    SinglyLinkedList(SinglyLinkedList&&) noexcept;
    SinglyLinkedList& operator=(SinglyLinkedList&&) noexcept;
    void push_front(const T& value);
    void push_back(const T& value);
    T pop_front();
    T pop_back();
    void remove_value(const T& value);
    bool find(const T& value) const;
    void insert_after(const T& target_value, const T& new_value);
    void insert_before(const T& target_value, const T& new_value);
    void remove_after(const T& target_value);
    void remove_before(const T& target_value);
    std::string serialize() const;
    void deserialize(const std::string& str);
};