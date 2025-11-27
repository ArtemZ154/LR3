#include "Set.h"
#include <functional> // Для std::hash

// --- Общая хеш-функция, использующая std::hash ---
template<typename T>
size_t Set<T>::manualHash(const T& value) const {
    // Эта реализация будет использоваться для всех типов, кроме std::string
    return std::hash<T>{}(value) % capacity;
}

// --- Ручная хеш-функция (DJB2) для std::string ---
template<>
size_t Set<std::string>::manualHash(const std::string& value) const {
    size_t hash = 5381;
    for (char c : value) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % capacity;
}

// --- Конструктор ---
template<typename T>
Set<T>::Set() : numElements(0), capacity(16) {
    table = new Entry[capacity];
}

// --- Деструктор ---
template<typename T>
Set<T>::~Set() {
    delete[] table;
}

// --- Rule of Five: Копирование и Перемещение ---

// Конструктор копирования
template<typename T>
Set<T>::Set(const Set& other) : numElements(other.numElements), capacity(other.capacity) {
    table = new Entry[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        table[i] = other.table[i];
    }
}

// Оператор присваивания копированием
template<typename T>
Set<T>& Set<T>::operator=(const Set& other) {
    if (this != &other) {
        delete[] table; // Освобождаем старую память
        numElements = other.numElements;
        capacity = other.capacity;
        table = new Entry[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            table[i] = other.table[i];
        }
    }
    return *this;
}

// Конструктор перемещения
template<typename T>
Set<T>::Set(Set&& other) noexcept
    : table(other.table), numElements(other.numElements), capacity(other.capacity) {
    other.table = nullptr;
    other.numElements = 0;
    other.capacity = 0;
}

// Оператор присваивания перемещением
template<typename T>
Set<T>& Set<T>::operator=(Set&& other) noexcept {
    if (this != &other) {
        delete[] table;
        table = other.table;
        numElements = other.numElements;
        capacity = other.capacity;

        other.table = nullptr;
        other.numElements = 0;
        other.capacity = 0;
    }
    return *this;
}

// --- Внутренние методы ---

template<typename T>
void Set<T>::rehash() {
    size_t oldCapacity = capacity;
    Entry* oldTable = table;

    capacity *= 2;
    numElements = 0;
    table = new Entry[capacity]; // Новый чистый массив

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable[i].state == ACTIVE) {
            add(oldTable[i].value); // Перевставляем в новую таблицу
        }
    }
    delete[] oldTable; // Удаляем старый массив
}

template<typename T>
void Set<T>::checkLoad() {
    if (static_cast<float>(numElements) / capacity >= maxLoadFactor) {
        rehash();
    }
}

// --- Основные операции O(1) ---

template<typename T>
void Set<T>::add(const T& value) {
    checkLoad();
    size_t idx = manualHash(value);
    size_t firstDeleted = -1;

    for (size_t i = 0; i < capacity; ++i) {
        if (table[idx].state == EMPTY) {
            // Нашли пустую ячейку. Если была удаленная, используем её.
            size_t insertIdx = (firstDeleted != -1) ? firstDeleted : idx;
            table[insertIdx].value = value;
            table[insertIdx].state = ACTIVE;
            numElements++;
            return;
        }

        if (table[idx].state == ACTIVE && table[idx].value == value) {
            return; // Элемент уже существует
        }

        if (table[idx].state == DELETED && firstDeleted == -1) {
            firstDeleted = idx; // Запоминаем первое место, куда можно вставить
        }

        idx = (idx + 1) % capacity; // Линейное пробирование
    }
}

template<typename T>
bool Set<T>::contains(const T& value) const {
    if (numElements == 0) return false;
    size_t idx = manualHash(value);

    for (size_t i = 0; i < capacity; ++i) {
        if (table[idx].state == EMPTY) {
            return false; // Элемента точно нет, т.к. прервалась цепочка
        }
        if (table[idx].state == ACTIVE && table[idx].value == value) {
            return true;
        }
        idx = (idx + 1) % capacity;
    }
    return false;
}

template<typename T>
void Set<T>::remove(const T& value) {
    if (numElements == 0) return;
    size_t idx = manualHash(value);

    for (size_t i = 0; i < capacity; ++i) {
        if (table[idx].state == EMPTY) {
            return; // Элемент не найден
        }
        if (table[idx].state == ACTIVE && table[idx].value == value) {
            table[idx].state = DELETED; // Помечаем как "удалено" (tombstone)
            numElements--;
            return;
        }
        idx = (idx + 1) % capacity;
    }
}

// --- Операции над множествами ---

template<typename T>
Set<T> Set<T>::set_union(const Set<T>& other) const {
    Set<T> result;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == ACTIVE) result.add(table[i].value);
    }
    for (size_t i = 0; i < other.capacity; ++i) {
        if (other.table[i].state == ACTIVE) result.add(other.table[i].value);
    }
    return result;
}

template<typename T>
Set<T> Set<T>::set_intersection(const Set<T>& other) const {
    Set<T> result;
    // Оптимизация: итерируем по меньшему множеству
    const Set<T>* smaller = (this->numElements < other.numElements) ? this : &other;
    const Set<T>* larger = (this->numElements < other.numElements) ? &other : this;

    for (size_t i = 0; i < smaller->capacity; ++i) {
        if (smaller->table[i].state == ACTIVE && larger->contains(smaller->table[i].value)) {
            result.add(smaller->table[i].value);
        }
    }
    return result;
}

template<typename T>
Set<T> Set<T>::set_difference(const Set<T>& other) const {
    Set<T> result;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == ACTIVE && !other.contains(table[i].value)) {
            result.add(table[i].value);
        }
    }
    return result;
}

// --- Вспомогательные методы ---

template<typename T>
size_t Set<T>::size() const {
    return numElements;
}

template<typename T>
bool Set<T>::empty() const {
    return numElements == 0;
}

template<typename T>
void Set<T>::clear() {
    delete[] table;
    capacity = 16;
    numElements = 0;
    table = new Entry[capacity]; // Новый пустой массив
}

// --- Получение элементов (без std::vector) ---
template<typename T>
T* Set<T>::getElements() const {
    if (numElements == 0) {
        return nullptr;
    }
    T* elements = new T[numElements];
    size_t j = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == ACTIVE) {
            elements[j++] = table[i].value;
        }
    }
    return elements;
}

// --- Сериализация ---

template<typename T>
std::string Set<T>::serialize() const {
    std::stringstream ss;
    bool first = true;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == ACTIVE) {
            if (!first) ss << " ";
            ss << table[i].value;
            first = false;
        }
    }
    return ss.str();
}

template<typename T>
void Set<T>::deserialize(const std::string& str) {
    clear();
    std::stringstream ss(str);
    T value;
    while (ss >> value) {
        add(value);
    }
}

// --- Явное инстанцирование ---
template class Set<std::string>;
template class Set<int>; // Добавлено для покрытия тестами
