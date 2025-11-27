#pragma once
#include <string>
#include <unordered_map>
#include <list>
#include <map>

/*
 * Реализация LFU (Least Frequently Used) Cache.
 * Требуется для Задания 7, Вариант 2.
 */
class LFUCache {
private:
    // Емкость кэша
    int capacity;
    // Текущий размер
    int size;
    // Минимальная частота для быстрой эвакуации
    int minFreq;

    // (Ключ -> {Значение, Частота})
    std::unordered_map<std::string, std::pair<std::string, int>> keyToValFreq;

    // (Частота -> Список Ключей в порядке LRU)
    std::map<int, std::list<std::string>> freqToKeys;

    // (Ключ -> Итератор на позицию в списке freqToKeys)
    std::unordered_map<std::string, std::list<std::string>::iterator> keyToIterator;

    // Внутренние вспомогательные методы
    void updateFrequency(const std::string& key);
    void evict();
    void internal_set(const std::string& key, const std::string& value, int freq);

public:
    // Конструктор
    LFUCache(int cap = 0);

    // Копирование запрещено
    LFUCache(const LFUCache&) = delete;
    LFUCache& operator=(const LFUCache&) = delete;

    // Перемещение разрешено (оставляем default в хедере, так как это стандартная практика для тривиальных реализаций)
    LFUCache(LFUCache&&) noexcept = default;
    LFUCache& operator=(LFUCache&&) noexcept = default;

    // Основные публичные методы
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value);

    // Сериализация
    std::string serialize() const;
    void deserialize(const std::string& str);
};