#include "gtest/gtest.h"
#include "../DataStructures/SinglyLinkedList.h"
#include <string>

class SinglyLinkedListTest : public ::testing::Test {
protected:
    SinglyLinkedList<std::string> list;
};

TEST_F(SinglyLinkedListTest, IsInitiallyEmpty) {
    EXPECT_EQ(list.serialize(), "");
    EXPECT_TRUE(list.find("a") == false);
}

TEST_F(SinglyLinkedListTest, PushFront) {
    list.push_front("b");
    list.push_front("a");
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, PushBack) {
    list.push_back("a");
    list.push_back("b");
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, PopFront) {
    list.push_back("a");
    list.push_back("b");
    std::string val = list.pop_front();
    EXPECT_EQ(val, "a");
    EXPECT_EQ(list.serialize(), "b");
}

TEST_F(SinglyLinkedListTest, PopBack) {
    list.push_back("a");
    list.push_back("b");
    std::string val = list.pop_back();
    EXPECT_EQ(val, "b");
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(SinglyLinkedListTest, PopBackSingleElement) {
    list.push_back("a");
    EXPECT_EQ(list.pop_back(), "a");
    EXPECT_EQ(list.serialize(), "");
}

TEST_F(SinglyLinkedListTest, PopOnEmptyThrows) {
    EXPECT_THROW(list.pop_front(), std::runtime_error);
    EXPECT_THROW(list.pop_back(), std::runtime_error);
}

TEST_F(SinglyLinkedListTest, Find) {
    list.push_back("a");
    list.push_back("b");
    EXPECT_TRUE(list.find("a"));
    EXPECT_TRUE(list.find("b"));
    EXPECT_FALSE(list.find("c"));
}

TEST_F(SinglyLinkedListTest, RemoveValue) {
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.remove_value("b");
    EXPECT_EQ(list.serialize(), "a c");
}

TEST_F(SinglyLinkedListTest, RemoveFirstValue) {
    list.push_back("a");
    list.push_back("b");
    list.remove_value("a");
    EXPECT_EQ(list.serialize(), "b");
}

TEST_F(SinglyLinkedListTest, RemoveLastValue) {
    list.push_back("a");
    list.push_back("b");
    list.remove_value("b");
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(SinglyLinkedListTest, RemoveFromEmpty) {
    list.remove_value("a");
    EXPECT_EQ(list.serialize(), "");
}

TEST_F(SinglyLinkedListTest, InsertAfter) {
    list.push_back("a");
    list.push_back("c");
    list.insert_after("a", "b");
    EXPECT_EQ(list.serialize(), "a b c");
}

TEST_F(SinglyLinkedListTest, InsertAfterLast) {
    list.push_back("a");
    list.insert_after("a", "b");
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, InsertAfterNonExistent) {
    list.push_back("a");
    list.insert_after("x", "b");
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(SinglyLinkedListTest, InsertBefore) {
    list.push_back("a");
    list.push_back("c");
    list.insert_before("c", "b");
    EXPECT_EQ(list.serialize(), "a b c");
}

TEST_F(SinglyLinkedListTest, InsertBeforeFirst) {
    list.push_back("b");
    list.insert_before("b", "a");
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, InsertBeforeNonExistent) {
    list.push_back("a");
    list.insert_before("x", "b");
    EXPECT_EQ(list.serialize(), "a");
}

TEST_F(SinglyLinkedListTest, InsertBeforeInEmpty) {
    list.insert_before("x", "b");
    EXPECT_EQ(list.serialize(), "");
}

TEST_F(SinglyLinkedListTest, RemoveAfter) {
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.remove_after("a");
    EXPECT_EQ(list.serialize(), "a c");
}

TEST_F(SinglyLinkedListTest, RemoveAfterNonExistent) {
    list.push_back("a");
    list.push_back("b");
    list.remove_after("x");
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, RemoveAfterLast) {
    list.push_back("a");
    list.push_back("b");
    list.remove_after("b"); // Nothing to remove
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, RemoveBefore) {
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.remove_before("c");
    EXPECT_EQ(list.serialize(), "a c");
}

TEST_F(SinglyLinkedListTest, RemoveBeforeFirst) {
    list.push_back("a");
    list.push_back("b");
    list.remove_before("b");
    EXPECT_EQ(list.serialize(), "b");
}

TEST_F(SinglyLinkedListTest, RemoveBeforeNonExistent) {
    list.push_back("a");
    list.push_back("b");
    list.remove_before("x");
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, RemoveBeforeWithTwoElements) {
    list.push_back("a");
    list.push_back("b");
    list.remove_before("a"); // Nothing to remove
    EXPECT_EQ(list.serialize(), "a b");
}

TEST_F(SinglyLinkedListTest, SerializationAndDeserialization) {
    list.push_back("one");
    list.push_back("two");
    list.push_back("three");
    std::string serialized = list.serialize();
    EXPECT_EQ(serialized, "one two three");

    SinglyLinkedList<std::string> newList;
    newList.deserialize(serialized);
    EXPECT_EQ(newList.serialize(), "one two three");
}
