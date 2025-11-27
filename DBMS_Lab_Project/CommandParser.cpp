#include "CommandParser.h"
#include <sstream>
#include <iterator>
std::vector<std::string> CommandParser::parse(const std::string& query) {
    std::stringstream ss(query);
    std::istream_iterator<std::string> begin(ss), end;
    return {begin, end};
}
