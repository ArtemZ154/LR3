#include "gtest/gtest.h"
#include "../DataStructures/Array.h"
#include <string>

TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, PushBackAndGet) {
    Array<std::string> arr;
    arr.push_back("hello");
    arr.push_back("world");
    ASSERT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), "hello");
    EXPECT_EQ(arr.get(1), "world");
}

TEST(ArrayTest, GetThrowsException) {
    Array<int> arr;
    EXPECT_THROW(arr.get(0), std::out_of_range);
    arr.push_back(10);
    EXPECT_THROW(arr.get(1), std::out_of_range);
}

TEST(ArrayTest, Set) {
    Array<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.set(1, 99);
    ASSERT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(1), 99);
}

TEST(ArrayTest, SetThrowsException) {
    Array<int> arr;
    EXPECT_THROW(arr.set(0, 10), std::out_of_range);
}

TEST(ArrayTest, Insert) {
    Array<std::string> arr;
    arr.push_back("a");
    arr.push_back("c");
    arr.insert(1, "b"); // a, b, c
    ASSERT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.get(0), "a");
    EXPECT_EQ(arr.get(1), "b");
    EXPECT_EQ(arr.get(2), "c");
}

TEST(ArrayTest, InsertAtBeginning) {
    Array<int> arr;
    arr.push_back(2);
    arr.insert(0, 1);
    ASSERT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), 1);
    EXPECT_EQ(arr.get(1), 2);
}

TEST(ArrayTest, InsertAtEnd) {
    Array<int> arr;
    arr.push_back(1);
    arr.insert(1, 2); // Same as push_back
    ASSERT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(1), 2);
}

TEST(ArrayTest, InsertThrowsException) {
    Array<int> arr;
    EXPECT_THROW(arr.insert(1, 100), std::out_of_range);
}

TEST(ArrayTest, Remove) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    arr.remove(1); // 10, 30
    ASSERT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 30);
}

TEST(ArrayTest, RemoveThrowsException) {
    Array<int> arr;
    EXPECT_THROW(arr.remove(0), std::out_of_range);
}

TEST(ArrayTest, Clear) {
    Array<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.clear();
    EXPECT_EQ(arr.size(), 0);
    EXPECT_THROW(arr.get(0), std::out_of_range);
}

TEST(ArrayTest, ResizeAndGrowth) {
    Array<int> arr;
    // Initial capacity is 0, first push_back resizes to 8
    arr.push_back(1);
    EXPECT_EQ(arr.size(), 1);
    for (int i = 2; i <= 8; ++i) {
        arr.push_back(i);
    }
    ASSERT_EQ(arr.size(), 8);
    // Next push_back should trigger resize to 16
    arr.push_back(9);
    EXPECT_EQ(arr.size(), 9);
}

TEST(ArrayTest, CopyConstructor) {
    Array<std::string> original;
    original.push_back("one");
    original.push_back("two");

    Array<std::string> copy = original;
    ASSERT_EQ(copy.size(), 2);
    EXPECT_EQ(copy.get(0), "one");
    EXPECT_EQ(copy.get(1), "two");

    // Modify original, copy should not change
    original.set(0, "new_one");
    EXPECT_EQ(copy.get(0), "one");
}

TEST(ArrayTest, CopyAssignmentOperator) {
    Array<int> original;
    original.push_back(10);
    original.push_back(20);

    Array<int> copy;
    copy.push_back(99);
    copy = original;

    ASSERT_EQ(copy.size(), 2);
    EXPECT_EQ(copy.get(0), 10);
    EXPECT_EQ(copy.get(1), 20);

    // Modify original, copy should not change
    original.set(0, 100);
    EXPECT_EQ(copy.get(0), 10);
}

TEST(ArrayTest, MoveConstructor) {
    Array<int> original;
    original.push_back(1);
    original.push_back(2);

    Array<int> moved = std::move(original);
    ASSERT_EQ(moved.size(), 2);
    EXPECT_EQ(moved.get(1), 2);

    // Original should be empty
    EXPECT_EQ(original.size(), 0);
}

TEST(ArrayTest, MoveAssignmentOperator) {
    Array<int> original;
    original.push_back(1);
    original.push_back(2);

    Array<int> moved;
    moved.push_back(99);
    moved = std::move(original);

    ASSERT_EQ(moved.size(), 2);
    EXPECT_EQ(moved.get(1), 2);

    // Original should be empty
    EXPECT_EQ(original.size(), 0);
}

TEST(ArrayTest, Serialization) {
    Array<std::string> arr;
    arr.push_back("a");
    arr.push_back("b c"); // Value with space
    arr.push_back("d");
    EXPECT_EQ(arr.serialize(), "a b c d");
}

TEST(ArrayTest, SerializationEmpty) {
    Array<int> arr;
    EXPECT_EQ(arr.serialize(), "");
}

TEST(ArrayTest, Deserialization) {
    Array<std::string> arr;
    arr.deserialize("hello world from test");
    ASSERT_EQ(arr.size(), 4);
    EXPECT_EQ(arr.get(0), "hello");
    EXPECT_EQ(arr.get(1), "world");
    EXPECT_EQ(arr.get(2), "from");
    EXPECT_EQ(arr.get(3), "test");
}

TEST(ArrayTest, DeserializationOverwrites) {
    Array<int> arr;
    arr.push_back(99);
    arr.deserialize("1 2 3");
    ASSERT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.get(0), 1);
}
