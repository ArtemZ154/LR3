#include "LFUCache.h"
#include <sstream>

LFUCache::LFUCache(int cap) : capacity(cap), size(0), minFreq(0) {}

void LFUCache::updateFrequency(const std::string& key) {
    int freq = keyToValFreq[key].second;
    auto it = keyToIterator[key];

    // 1. Удаляем ключ из списка старой частоты
    freqToKeys[freq].erase(it);

    // 2. Если список старой частоты опустел и это была minFreq,
    //    увеличиваем minFreq.
    if (freqToKeys[freq].empty() && freq == minFreq) {
        minFreq++;
    }

    // 3. Увеличиваем частоту ключа
    freq++;
    keyToValFreq[key].second = freq;

    // 4. Добавляем ключ в список новой частоты (в конец, т.к. он "recently used")
    freqToKeys[freq].push_back(key);
    keyToIterator[key] = --freqToKeys[freq].end();
}

void LFUCache::evict() {
    if (freqToKeys.empty()) return;

    // 1. Находим ключ для удаления (первый в списке minFreq, т.к. он LRU)
    std::string keyToEvict = freqToKeys[minFreq].front();
    freqToKeys[minFreq].pop_front();

    // 2. Удаляем его из всех карт
    keyToValFreq.erase(keyToEvict);
    keyToIterator.erase(keyToEvict);
}

void LFUCache::internal_set(const std::string& key, const std::string& value, int freq) {
    keyToValFreq[key] = {value, freq};
    freqToKeys[freq].push_back(key);
    keyToIterator[key] = --freqToKeys[freq].end();
    // При десериализации важно правильно установить minFreq
    if (minFreq == 0 || freq < minFreq) {
        minFreq = freq;
    }
    size++;
}

std::string LFUCache::get(const std::string& key) {
    if (keyToValFreq.find(key) == keyToValFreq.end()) {
        return "-1";
    }

    updateFrequency(key);
    return keyToValFreq[key].first;
}

void LFUCache::set(const std::string& key, const std::string& value) {
    if (capacity <= 0) return;

    if (keyToValFreq.find(key) != keyToValFreq.end()) {
        // Ключ уже существует, просто обновляем значение и частоту
        keyToValFreq[key].first = value;
        updateFrequency(key);
    } else {
        // Новый ключ
        if (size >= capacity) {
            // Кэш полон, нужно удалить самый редкий элемент
            evict();
            size--;
        }

        // Добавляем новый элемент с частотой 1
        keyToValFreq[key] = {value, 1};
        freqToKeys[1].push_back(key);
        keyToIterator[key] = --freqToKeys[1].end();
        minFreq = 1; // Сбрасываем minFreq на 1 для нового элемента
        size++;
    }
}

std::string LFUCache::serialize() const {
    std::stringstream ss;
    // 1. Сохраняем ёмкость
    ss << capacity;
    // 2. Сохраняем все тройки (ключ, значение, частота)
    for (const auto& pair : keyToValFreq) {
        const std::string& key = pair.first;
        const std::string& val = pair.second.first;
        int freq = pair.second.second;
        ss << " " << key << " " << val << " " << freq;
    }
    return ss.str();
}

void LFUCache::deserialize(const std::string& str) {
    // 1. Очищаем текущее состояние
    keyToValFreq.clear();
    freqToKeys.clear();
    keyToIterator.clear();
    size = 0;
    minFreq = 0;
    capacity = 0;

    if (str.empty()) return;

    std::stringstream ss(str);

    // 2. Загружаем ёмкость
    ss >> capacity;
    if (capacity <= 0) return;

    // 3. Загружаем тройки (ключ, значение, частота)
    std::string key, val;
    int freq;
    while (ss >> key >> val >> freq) {
        internal_set(key, val, freq);
    }
}