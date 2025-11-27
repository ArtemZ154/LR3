#include "StorageManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
StorageManager::StorageManager(const std::string& path) : filepath(path) {}
void StorageManager::load(DBMS& dbms) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    dbms.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type, name, data;
        ss >> type >> name;
        std::getline(ss, data);
        if (!data.empty() && data[0] == ' ') data = data.substr(1);
        dbms.loadStructure(type, name, data);
    }
}
void StorageManager::save(const DBMS& dbms) {
    std::ofstream file(filepath, std::ios::trunc);
    if (!file.is_open()) throw std::runtime_error("Could not open file for writing: " + filepath);
    file << dbms.serializeAll();
}
