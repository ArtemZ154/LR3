#pragma once

#include <string>
#include <stdexcept>
#include <sstream>

template<typename T>
class Set {
private:
    enum State : char {
        EMPTY,
        ACTIVE,
        DELETED
    };

    struct Entry {
        T value;
        State state = EMPTY;
    };

    Entry* table;          // Сырой указатель на массив
    size_t numElements;    // Количество элементов
    size_t capacity;       // Размер массива
    const float maxLoadFactor = 0.6f;

    size_t manualHash(const T& value) const;
    void rehash();
    void checkLoad();

public:
    Set();
    ~Set();

    Set(const Set& other);
    Set& operator=(const Set& other);
    Set(Set&& other) noexcept;
    Set& operator=(Set&& other) noexcept;

    void add(const T& value);
    void remove(const T& value);
    bool contains(const T& value) const;

    Set<T> set_union(const Set<T>& other) const;
    Set<T> set_intersection(const Set<T>& other) const;
    Set<T> set_difference(const Set<T>& other) const;

    size_t size() const;
    bool empty() const;
    void clear();

    std::string serialize() const;
    void deserialize(const std::string& str);

    T* getElements() const;
};