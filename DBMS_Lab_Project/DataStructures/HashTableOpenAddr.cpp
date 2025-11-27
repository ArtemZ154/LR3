#include "HashTableOpenAddr.h"
#include <stdexcept>
#include <sstream>

// --- Хеш-функция (DJB2) ---
size_t HashTableOpenAddr::manualHash(const std::string& key) const {
    size_t hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

// --- Конструктор / Деструктор ---
HashTableOpenAddr::HashTableOpenAddr(size_t cap)
    : capacity(cap), numElements(0) {
    table = new Entry[capacity];
}

HashTableOpenAddr::~HashTableOpenAddr() {
    delete[] table;
}

// --- rehash ---
void HashTableOpenAddr::rehash() {
    size_t oldCapacity = capacity;
    Entry* oldTable = table;

    capacity *= 2;
    numElements = 0;
    table = new Entry[capacity]; // Новая чистая таблица

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable[i].state == ACTIVE) {
            // Вставляем старые данные в новую таблицу
            put(oldTable[i].key, oldTable[i].value);
        }
    }
    delete[] oldTable;
}

// --- put (Добавить/Обновить) ---
void HashTableOpenAddr::put(const std::string& key, const std::string& value) {
    if (static_cast<float>(numElements) / capacity >= maxLoadFactor) {
        rehash();
    }

    size_t index = manualHash(key);
    size_t firstDeleted = -1;

    for (size_t i = 0; i < capacity; ++i) {
        if (table[index].state == ACTIVE && table[index].key == key) {
            // Ключ найден, обновляем значение
            table[index].value = value;
            return;
        }

        if (table[index].state == DELETED && firstDeleted == -1) {
            firstDeleted = index; // Запоминаем "надгробие"
        }

        if (table[index].state == EMPTY) {
            // Нашли пустое место. Используем его или "надгробие", если было.
            size_t insertIndex = (firstDeleted != -1) ? firstDeleted : index;
            table[insertIndex].key = key;
            table[insertIndex].value = value;
            table[insertIndex].state = ACTIVE;
            numElements++;
            return;
        }

        index = (index + 1) % capacity; // Линейное пробирование
    }
}

// --- get (Получить) ---
std::string HashTableOpenAddr::get(const std::string& key) const {
    size_t index = manualHash(key);

    for (size_t i = 0; i < capacity; ++i) {
        if (table[index].state == EMPTY) {
            break; // Элемента точно нет
        }
        if (table[index].state == ACTIVE && table[index].key == key) {
            return table[index].value; // Найдено
        }
        index = (index + 1) % capacity; // Ищем дальше
    }

    throw std::out_of_range("Key not found");
}

bool HashTableOpenAddr::get(const std::string& key, std::string& value) const {
    size_t index = manualHash(key);

    for (size_t i = 0; i < capacity; ++i) {
        if (table[index].state == EMPTY) {
            return false; // Элемента точно нет
        }
        if (table[index].state == ACTIVE && table[index].key == key) {
            value = table[index].value; // Найдено
            return true;
        }
        index = (index + 1) % capacity; // Ищем дальше
    }

    return false; // Не найдено после полного прохода
}


// --- remove (Удалить) ---
void HashTableOpenAddr::remove(const std::string& key) {
    size_t index = manualHash(key);

    for (size_t i = 0; i < capacity; ++i) {
        if (table[index].state == EMPTY) {
            return; // Элемента нет
        }
        if (table[index].state == ACTIVE && table[index].key == key) {
            table[index].state = DELETED;
            numElements--;
            return;
        }
        index = (index + 1) % capacity;
    }
}

void HashTableOpenAddr::clear() {
    delete[] table;
    table = new Entry[capacity]; // Используем *текущую* ёмкость
    numElements = 0;
}

std::string HashTableOpenAddr::serialize() const {
    std::stringstream ss;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == ACTIVE) {
            // Формат: "key:value "
            ss << table[i].key << ":" << table[i].value << " ";
        }
    }
    return ss.str();
}

void HashTableOpenAddr::deserialize(const std::string& str) {
    clear();
    std::stringstream ss(str);
    std::string pair_str;

    while (ss >> pair_str) {
        size_t colon_pos = pair_str.find(':');
        if (colon_pos == std::string::npos) continue;
        std::string key = pair_str.substr(0, colon_pos);
        std::string value = pair_str.substr(colon_pos + 1);
        put(key, value);
    }
}