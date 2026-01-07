#include "HashTableChaining.h"
#include <stdexcept>
#include <sstream>
#include "../BinaryUtils.h"

size_t HashTableChaining::manualHash(const std::string& key) const {
    size_t hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

HashTableChaining::HashTableChaining(size_t cap) : capacity(cap) {
    table.resize(capacity);
}

void HashTableChaining::put(const std::string& key, const std::string& value) {
    size_t index = manualHash(key);
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    table[index].push_back({key, value});
}

std::string HashTableChaining::get(const std::string& key) const {
    size_t index = manualHash(key);
    for (const auto& pair : table[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw std::out_of_range("Key not found");
}

void HashTableChaining::remove(const std::string& key) {
    size_t index = manualHash(key);
    auto& chain = table[index];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->first == key) {
            chain.erase(it);
            return;
        }
    }
}



std::string HashTableChaining::serialize() const {
    std::stringstream ss;
    size_t total = 0;
    for(const auto& chain : table) total += chain.size();
    writeSize(ss, total);

    for (const auto& chain : table) {
        for (const auto& pair : chain) {
            writeString(ss, pair.first);
            writeString(ss, pair.second);
        }
    }
    return ss.str();
}

void HashTableChaining::deserialize(const std::string& str) {
    table.clear();
    table.resize(capacity);
    if(str.empty()) return;
    std::stringstream ss(str);
    size_t count;
    readSize(ss, count);

    for(size_t i = 0; i < count; ++i) {
        std::string key, value;
        readString(ss, key);
        readString(ss, value);
        put(key, value);
    }
}