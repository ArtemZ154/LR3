#include "gtest/gtest.h"
#include "../DataStructures/DoublyLinkedList.h"
#include <string>

class DoublyLinkedListTest : public ::testing::Test {
protected:
    DoublyLinkedList<std::string> list;
};

TEST_F(DoublyLinkedListTest, IsInitiallyEmpty) {
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.serialize(), "");
    EXPECT_FALSE(list.find("a"));
}

TEST_F(DoublyLinkedListTest, PushFront) {
    list.push_front("b");
    list.push_front("a");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(DoublyLinkedListTest, PushBack) {
    list.push_back("a");
    list.push_back("b");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(DoublyLinkedListTest, PopFront) {
    list.push_back("a");
    list.push_back("b");
    std::string val = list.pop_front();
    EXPECT_EQ(val, "a");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "b");
}

TEST_F(DoublyLinkedListTest, PopBack) {
    list.push_back("a");
    list.push_back("b");
    std::string val = list.pop_back();
    EXPECT_EQ(val, "b");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(DoublyLinkedListTest, PopOnEmptyThrows) {
    EXPECT_THROW(list.pop_front(), std::runtime_error);
    EXPECT_THROW(list.pop_back(), std::runtime_error);
}

TEST_F(DoublyLinkedListTest, Find) {
    list.push_back("a");
    list.push_back("b");
    EXPECT_TRUE(list.find("a"));
    EXPECT_TRUE(list.find("b"));
    EXPECT_FALSE(list.find("c"));
}

TEST_F(DoublyLinkedListTest, RemoveValue) {
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.remove_value("b");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.serialize(), "a c");
}

TEST_F(DoublyLinkedListTest, RemoveNonExistentValue) {
    list.push_back("a");
    list.remove_value("x");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(DoublyLinkedListTest, InsertAfter) {
    list.push_back("a");
    list.push_back("c");
    list.insert_after("a", "b");
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.serialize(), "a b c");
}

TEST_F(DoublyLinkedListTest, InsertAfterNonExistent) {
    list.push_back("a");
    list.insert_after("x", "b");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(DoublyLinkedListTest, InsertBefore) {
    list.push_back("a");
    list.push_back("c");
    list.insert_before("c", "b");
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.serialize(), "a b c");
}

TEST_F(DoublyLinkedListTest, InsertBeforeNonExistent) {
    list.push_back("a");
    list.insert_before("x", "b");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(DoublyLinkedListTest, RemoveAfter) {
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.remove_after("a");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.serialize(), "a c");
}

TEST_F(DoublyLinkedListTest, RemoveAfterNonExistent) {
    list.push_back("a");
    list.remove_after("x");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(DoublyLinkedListTest, RemoveBefore) {
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.remove_before("c");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.serialize(), "a c");
}

TEST_F(DoublyLinkedListTest, RemoveBeforeNonExistent) {
    list.push_back("a");
    list.remove_before("x");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(DoublyLinkedListTest, ComplexScenario) {
    list.push_back("1");
    list.push_back("2");
    list.push_front("0"); // 0 1 2
    list.pop_back();      // 0 1
    list.push_back("3");  // 0 1 3
    list.insert_after("1", "5"); // 0 1 5 3
    list.remove_value("0"); // 1 5 3
    list.pop_front();     // 5 3
    EXPECT_EQ(list.serialize(), "5 3");
    EXPECT_EQ(list.size(), 2);
}

TEST_F(DoublyLinkedListTest, SerializationAndDeserialization) {
    list.push_back("one");
    list.push_back("two");
    list.push_back("three");
    std::string serialized = list.serialize();
    EXPECT_EQ(serialized, "one two three");

    DoublyLinkedList<std::string> newList;
    newList.deserialize(serialized);
    EXPECT_EQ(newList.serialize(), "one two three");
    EXPECT_EQ(newList.size(), 3);
}
