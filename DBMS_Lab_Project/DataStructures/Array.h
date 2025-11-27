#pragma once
#include <string>

template<typename T>
class Array {
private:
    void resize(size_t new_capacity);
    T* data;
    size_t current_size;
    size_t capacity;

public:
    Array();
    ~Array();
    Array(const Array& other);
    Array& operator=(const Array& other);
    Array(Array&& other) noexcept;
    Array& operator=(Array&& other) noexcept;

    void push_back(const T& value);
    void insert(size_t index, const T& value);
    T get(size_t index) const;
    void remove(size_t index);
    void set(size_t index, const T& value);
    size_t size() const;

    void clear();

    std::string serialize() const;
    void deserialize(const std::string& str);
};