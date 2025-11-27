#include "gtest/gtest.h"
#include "../CommandParser.h"

TEST(CommandParserTest, BasicParsing) {
    std::string query = "MPUSH my_array val1 val2";
    std::vector<std::string> expected = {"MPUSH", "my_array", "val1", "val2"};
    EXPECT_EQ(CommandParser::parse(query), expected);
}

TEST(CommandParserTest, EmptyQuery) {
    std::string query = "";
    std::vector<std::string> expected = {};
    EXPECT_EQ(CommandParser::parse(query), expected);
}

TEST(CommandParserTest, QueryWithExtraSpaces) {
    std::string query = "  LPUSHB   my_list   item1  ";
    std::vector<std::string> expected = {"LPUSHB", "my_list", "item1"};
    EXPECT_EQ(CommandParser::parse(query), expected);
}
