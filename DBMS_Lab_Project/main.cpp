#include <iostream>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "CommandParser.h"
#include "DBMS.h"
#include "StorageManager.h"

int main(int argc, char* argv[]) {
    std::string filepath = "database.data";
    std::string query = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filepath = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }
    if (query.empty()) {
        std::cerr << "Usage: ./dbms --file <filename> --query '<command>'" << std::endl;
        return 1;
    }
    try {
        DBMS dbms;
        StorageManager storage(filepath);
        storage.load(dbms);
        auto command = CommandParser::parse(query);
        std::string result = dbms.execute(command);
        std::cout << result << std::endl;
        storage.save(dbms);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
