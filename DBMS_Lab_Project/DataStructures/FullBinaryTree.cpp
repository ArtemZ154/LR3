#include "FullBinaryTree.h"
#include <queue>
#include <vector>
#include <stack>

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
void FullBinaryTree<T>::serializeRecursive(const Node* node, int depth, std::stringstream& ss) const {
    if (!node) return;

    // Добавляем отступ, значение и разделитель '|'
    ss << std::string(depth, '.') << node->data << "|";

    // Рекурсивно для левого и правого потомка
    serializeRecursive(node->left.get(), depth + 1, ss);
    serializeRecursive(node->right.get(), depth + 1, ss);
}

template<typename T>
std::string FullBinaryTree<T>::serialize() const {
    std::stringstream ss;
    serializeRecursive(root.get(), 0, ss);
    std::string result = ss.str();
    // Убираем последний лишний '|'
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}


// --- НОВАЯ ЛОГИКА ДЕСЕРИАЛИЗАЦИИ (с отступами) ---
template<typename T>
void FullBinaryTree<T>::deserialize(const std::string& str) {
    root.reset();
    if (str.empty()) return;

    std::stringstream ss(str);
    std::string segment;
    std::vector<std::string> lines;
    while(std::getline(ss, segment, '|')) {
        lines.push_back(segment);
    }

    if (lines.empty()) return;

    // Стек для хранения родительских узлов на каждом уровне
    std::vector<Node*> parent_stack;

    // Создаем корень
    T data;
    std::stringstream converter(lines[0]);
    converter >> data;
    root = std::make_unique<Node>(data);
    parent_stack.push_back(root.get());

    for (size_t i = 1; i < lines.size(); ++i) {
        size_t depth = lines[i].find_first_not_of('.');
        std::string value_str = lines[i].substr(depth);

        converter.str(value_str);
        converter.clear();
        converter >> data;
        auto newNode = std::make_unique<Node>(data);

        // Находим родителя на предыдущем уровне
        Node* parent = parent_stack[depth - 1];

        // Добавляем как левого или правого потомка
        if (!parent->left) {
            parent->left = std::move(newNode);
        } else {
            parent->right = std::move(newNode);
        }

        // Обновляем стек родителей
        if (parent_stack.size() <= depth) {
            parent_stack.resize(depth + 1);
        }
        parent_stack[depth] = (parent->right) ? parent->right.get() : parent->left.get();
    }
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