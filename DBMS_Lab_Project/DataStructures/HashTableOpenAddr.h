#pragma once
#include <string>
#include <sstream>

class HashTableOpenAddr {
private:
    enum State : char { EMPTY, ACTIVE, DELETED };
    struct Entry {
        std::string key;
        std::string value;
        State state = EMPTY;
    };

    Entry* table;
    size_t capacity;
    size_t numElements;
    const float maxLoadFactor = 0.6f;

    size_t manualHash(const std::string& key) const;
    void rehash();

public:
    HashTableOpenAddr(size_t cap = 16);
    ~HashTableOpenAddr();
    HashTableOpenAddr(const HashTableOpenAddr& other);
    HashTableOpenAddr& operator=(const HashTableOpenAddr& other);
    HashTableOpenAddr(HashTableOpenAddr&& other) noexcept;
    HashTableOpenAddr& operator=(HashTableOpenAddr&& other) noexcept;

    void put(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;
    bool get(const std::string& key, std::string& value) const;
    void remove(const std::string& key);

    // --- ДОБАВЛЕНО ДЛЯ DBMS ---
    void clear();
    std::string serialize() const;
    void deserialize(const std::string& str);
};