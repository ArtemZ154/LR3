#include "FullBinaryTree.h"
#include <queue>
#include <vector>
#include <stack>
#include <functional>
#include "../BinaryUtils.h"

template<typename T> struct FullBinaryTree<T>::Node {
    T data;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node(T val) : data(val), left(nullptr), right(nullptr) {}
};

// --- Конструкторы и деструктор (остаются без изменений) ---
template<typename T> FullBinaryTree<T>::FullBinaryTree() = default;
template<typename T> FullBinaryTree<T>::~FullBinaryTree() = default;
template<typename T> FullBinaryTree<T>::FullBinaryTree(FullBinaryTree&&) noexcept = default;
template<typename T> FullBinaryTree<T>& FullBinaryTree<T>::operator=(FullBinaryTree&&) noexcept = default;


// --- Основные публичные методы (интерфейс не меняется) ---
template<typename T> void FullBinaryTree<T>::insert(const T& value) {
    if (!root) { root = std::make_unique<Node>(value); return; }
    std::queue<Node*> q; q.push(root.get());
    while (!q.empty()) {
        Node* current = q.front(); q.pop();
        if (!current->left) { current->left = std::make_unique<Node>(value); return; } else { q.push(current->left.get()); }
        if (!current->right) { current->right = std::make_unique<Node>(value); return; } else { q.push(current->right.get()); }
    }
}

template<typename T> bool FullBinaryTree<T>::find(const T& value) const { return findRecursive(root.get(), value); }
template<typename T> bool FullBinaryTree<T>::isFull() const { return isFullRecursive(root.get()); }


// --- НОВАЯ ЛОГИКА СЕРИАЛИЗАЦИИ (с отступами) ---
template<typename T>
void FullBinaryTree<T>::serializeRecursive(const Node* node, std::stringstream& ss) const {
    unsigned char hasNode = node ? 1 : 0;
    ss.write(reinterpret_cast<char*>(&hasNode), 1);
    if(node) {
        writeValue(ss, node->data);
        serializeRecursive(node->left.get(), ss);
        serializeRecursive(node->right.get(), ss);
    }
}

template<typename T>
std::string FullBinaryTree<T>::serialize() const {
    std::stringstream ss;
    serializeRecursive(root.get(), ss);
    return ss.str();
}


// --- НОВАЯ ЛОГИКА ДЕСЕРИАЛИЗАЦИИ (с отступами) ---
template<typename T>
void FullBinaryTree<T>::deserialize(const std::string& str) {
    root.reset();
    if(str.empty()) return;

    std::stringstream ss(str);
    std::function<std::unique_ptr<Node>()> readNode = [&]() -> std::unique_ptr<Node> {
        unsigned char hasNode;
        if(!ss.read(reinterpret_cast<char*>(&hasNode), 1)) return nullptr;
        if(hasNode == 0) return nullptr;

        T val;
        readValue(ss, val);
        auto node = std::make_unique<Node>(val);
        node->left = readNode();
        node->right = readNode();
        return node;
    };
    root = readNode();
}


// --- Вспомогательные рекурсивные функции (остаются без изменений) ---
template<typename T> bool FullBinaryTree<T>::isFullRecursive(const Node* node) const {
    if (!node) return true;
    if (!node->left && !node->right) return true;
    if (node->left && node->right) return isFullRecursive(node->left.get()) && isFullRecursive(node->right.get());
    return false;
}

template<typename T> bool FullBinaryTree<T>::findRecursive(const Node* node, const T& value) const {
    if (!node) return false;
    if (node->data == value) return true;
    return findRecursive(node->left.get(), value) || findRecursive(node->right.get(), value);
}

// --- Явное инстанцирование (остаётся без изменений) ---
template class FullBinaryTree<std::string>;