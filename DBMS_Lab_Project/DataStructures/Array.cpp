#include "Array.h"
#include <sstream>
#include "../BinaryUtils.h"
#include <stdexcept>
#include <algorithm>

// --- Конструктор и деструктор ---
template<typename T>
Array<T>::Array() : data(nullptr), current_size(0), capacity(0) {}

template<typename T>
Array<T>::~Array() {
    delete[] data;
}

template<typename T>
Array<T>::Array(const Array& other) : data(nullptr), current_size(0), capacity(0) {
    if (other.capacity > 0) {
        data = new T[other.capacity];
        std::copy(other.data, other.data + other.current_size, data);
        current_size = other.current_size;
        capacity = other.capacity;
    }
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this == &other) return *this;
    delete[] data;
    data = nullptr;
    current_size = 0;
    capacity = 0;
    if (other.capacity > 0) {
        data = new T[other.capacity];
        std::copy(other.data, other.data + other.current_size, data);
        current_size = other.current_size;
        capacity = other.capacity;
    }
    return *this;
}

template<typename T>
Array<T>::Array(Array&& other) noexcept
    : data(other.data), current_size(other.current_size), capacity(other.capacity) {
    other.data = nullptr;
    other.current_size = 0;
    other.capacity = 0;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data = other.data;
    current_size = other.current_size;
    capacity = other.capacity;
    other.data = nullptr;
    other.current_size = 0;
    other.capacity = 0;
    return *this;
}

template<typename T>
void Array<T>::resize(size_t new_capacity) {
    T* new_data = new T[new_capacity];
    if (data) {
        // Убедимся, что не копируем больше, чем есть
        size_t size_to_copy = std::min(current_size, new_capacity);
        std::copy(data, data + size_to_copy, new_data);
        delete[] data;
    }
    data = new_data;
    capacity = new_capacity;
    // Обновим current_size, если он стал больше новой ёмкости
    if (current_size > new_capacity) {
        current_size = new_capacity;
    }
}

// --- Публичные методы ---
template<typename T>
void Array<T>::push_back(const T& value) {
    if (current_size >= capacity) {
        resize(capacity == 0 ? 8 : capacity * 2); // Удваиваем ёмкость при необходимости
    }
    data[current_size++] = value;
}

template<typename T>
void Array<T>::insert(size_t index, const T& value) {
    if (index > current_size) throw std::out_of_range("Index out of range");
    if (current_size >= capacity) {
        resize(capacity == 0 ? 8 : capacity * 2);
    }
    // Сдвигаем элементы вправо
    for (size_t i = current_size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    current_size++;
}

template<typename T>
T Array<T>::get(size_t index) const {
    if (index >= current_size) throw std::out_of_range("Index out of range");
    return data[index];
}

template<typename T>
void Array<T>::remove(size_t index) {
    if (index >= current_size) throw std::out_of_range("Index out of range");
    // Сдвигаем элементы влево
    for (size_t i = index; i < current_size - 1; ++i) {
        data[i] = data[i + 1];
    }
    current_size--;
}

template<typename T>
void Array<T>::set(size_t index, const T& value) {
    if (index >= current_size) throw std::out_of_range("Index out of range");
    data[index] = value;
}

template<typename T>
size_t Array<T>::size() const {
    return current_size;
}

template<typename T>
void Array<T>::clear() {
    delete[] data;
    data = nullptr;
    current_size = 0;
    capacity = 0;
}
// --- КОНЕЦ ДОБАВЛЕНИЯ ---

template<typename T>
std::string Array<T>::serialize() const {
    std::stringstream ss;
    writeSize(ss, current_size);
    for (size_t i = 0; i < current_size; ++i) {
        writeValue(ss, data[i]);
    }
    return ss.str();
}

template<typename T>
void Array<T>::deserialize(const std::string& str) {
    clear(); // --- ИЗМЕНЕНО: теперь используем clear() ---

    if (str.empty()) return;
    std::stringstream ss(str);
    size_t count = 0;
    readSize(ss, count);

    for(size_t i = 0; i < count; ++i) {
        T item;
        readValue(ss, item);
        push_back(item);
    }
}

// --- Явное инстанцирование ---
template class Array<std::string>;
template class Array<int>; // Добавлено для ЛР