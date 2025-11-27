#include "gtest/gtest.h"
#include "../DataStructures/Stack.h"
#include <string>

TEST(StackTest, IsInitiallyEmpty) {
    Stack<int> s;
    EXPECT_TRUE(s.empty());
}

TEST(StackTest, PushAndPeek) {
    Stack<std::string> s;
    s.push("first");
    s.push("second");
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.peek(), "second");
}

TEST(StackTest, Pop) {
    Stack<int> s;
    s.push(10);
    s.push(20);
    int val = s.pop();
    EXPECT_EQ(val, 20);
    EXPECT_EQ(s.peek(), 10);
}

TEST(StackTest, PopUntilEmpty) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.pop();
    s.pop();
    EXPECT_TRUE(s.empty());
}

TEST(StackTest, PopOrPeekOnEmptyThrowsException) {
    Stack<int> s;
    EXPECT_THROW(s.pop(), std::runtime_error);
    EXPECT_THROW(s.peek(), std::runtime_error);
}

TEST(StackTest, MoveConstructor) {
    Stack<int> original;
    original.push(1);
    original.push(2);

    Stack<int> moved = std::move(original);
    EXPECT_FALSE(moved.empty());
    EXPECT_EQ(moved.pop(), 2);
    EXPECT_EQ(moved.pop(), 1);
    EXPECT_TRUE(moved.empty());

    // Original should be empty and safe to use
    EXPECT_TRUE(original.empty());
    original.push(3);
    EXPECT_EQ(original.pop(), 3);
}

TEST(StackTest, MoveAssignmentOperator) {
    Stack<int> original;
    original.push(10);
    original.push(20);

    Stack<int> moved;
    moved.push(99); // Some initial state
    moved = std::move(original);

    EXPECT_EQ(moved.pop(), 20);
    EXPECT_EQ(moved.pop(), 10);
    EXPECT_TRUE(moved.empty());

    // Original should be empty
    EXPECT_TRUE(original.empty());
}

TEST(StackTest, Serialization) {
    Stack<std::string> s;
    s.push("c");
    s.push("b");
    s.push("a");
    // Serializes from top to bottom: a, then b, then c
    EXPECT_EQ(s.serialize(), "a b c");
}

TEST(StackTest, SerializationEmpty) {
    Stack<int> s;
    EXPECT_EQ(s.serialize(), "");
}

TEST(StackTest, Deserialization) {
    Stack<std::string> s;
    // The string represents the stack from top to bottom
    s.deserialize("top middle bottom");
    
    ASSERT_FALSE(s.empty());
    EXPECT_EQ(s.pop(), "top");
    EXPECT_EQ(s.pop(), "middle");
    EXPECT_EQ(s.pop(), "bottom");
    EXPECT_TRUE(s.empty());
}

TEST(StackTest, DeserializationOverwrites) {
    Stack<int> s;
    s.push(99);
    s.deserialize("1 2 3"); // top is 1
    
    ASSERT_FALSE(s.empty());
    EXPECT_EQ(s.pop(), 1);
    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.pop(), 3);
}
