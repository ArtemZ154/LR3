#pragma once
#include <string>
#include "DBMS.h"
class DBMS;
class StorageManager {
public:
    StorageManager(const std::string& filepath);
    void load(DBMS& dbms);
    void save(const DBMS& dbms);
private:
    std::string filepath;
};
