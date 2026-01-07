#include "StorageManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "BinaryUtils.h"
StorageManager::StorageManager(const std::string& path) : filepath(path) {}
void StorageManager::load(DBMS& dbms) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return;
    dbms.clear();
    
    // Check if file is empty
    file.peek();
    if(file.eof()) return;

    size_t total = 0;
    readSize(file, total);

    for (size_t i = 0; i < total; ++i) {
        if (file.peek() == EOF) break;
        std::string type, name, data;
        readString(file, type);
        readString(file, name);
        readString(file, data);
        dbms.loadStructure(type, name, data);
    }
}
void StorageManager::save(const DBMS& dbms) {
    std::ofstream file(filepath, std::ios::trunc | std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Could not open file for writing: " + filepath);
    std::string content = dbms.serializeAll();
    file.write(content.data(), content.size());
}
