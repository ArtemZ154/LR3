#pragma once

#include <string>
#include <vector>
#include <list>
#include <utility> // для std::pair
#include <sstream>

class HashTableChaining {
private:
    std::vector<std::list<std::pair<std::string, std::string>>> table;
    size_t capacity;
    size_t manualHash(const std::string& key) const;

public:
    HashTableChaining(size_t cap = 16);

    void put(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;
    void remove(const std::string& key);

    std::string serialize() const;
    void deserialize(const std::string& str);
};