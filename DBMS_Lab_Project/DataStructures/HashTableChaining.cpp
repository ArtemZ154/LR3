#include "HashTableChaining.h"
#include <stdexcept>
#include <sstream>

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
    for (const auto& chain : table) {
        for (const auto& pair : chain) {
            // Формат: "key:value "
            ss << pair.first << ":" << pair.second << " ";
        }
    }
    return ss.str();
}

void HashTableChaining::deserialize(const std::string& str) {
    table.clear();
    table.resize(capacity);
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