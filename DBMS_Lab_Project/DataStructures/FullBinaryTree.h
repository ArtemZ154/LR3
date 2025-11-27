#pragma once
#include <string>
#include <memory>
#include <sstream>

template<typename T>
class FullBinaryTree {
private:
    struct Node;
    std::unique_ptr<Node> root;
    void serializeRecursive(const Node* node, int depth, std::stringstream& ss) const;
    bool isFullRecursive(const Node* node) const;
    bool findRecursive(const Node* node, const T& value) const;

public:
    FullBinaryTree();
    ~FullBinaryTree();
    FullBinaryTree(FullBinaryTree&&) noexcept;
    FullBinaryTree& operator=(FullBinaryTree&&) noexcept;
    void insert(const T& value);
    bool find(const T& value) const;
    bool isFull() const;
    std::string serialize() const;
    void deserialize(const std::string& str);
};