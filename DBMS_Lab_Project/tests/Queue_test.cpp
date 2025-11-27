#include "gtest/gtest.h"
#include "../DataStructures/Queue.h"
#include <string>

TEST(QueueTest, IsInitiallyEmpty) {
    Queue<int> q;
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, PushAndPop) {
    Queue<std::string> q;
    q.push("first");
    q.push("second");
    EXPECT_FALSE(q.empty());
    
    EXPECT_EQ(q.pop(), "first");
    EXPECT_EQ(q.pop(), "second");
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, PopOnEmptyThrowsException) {
    Queue<int> q;
    EXPECT_THROW(q.pop(), std::runtime_error);
}

TEST(QueueTest, OrderIsPreserved) {
    Queue<int> q;
    for (int i = 0; i < 100; ++i) {
        q.push(i);
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(q.pop(), i);
    }
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, MoveConstructor) {
    Queue<int> original;
    original.push(1);
    original.push(2);

    Queue<int> moved = std::move(original);
    EXPECT_FALSE(moved.empty());
    EXPECT_EQ(moved.pop(), 1);
    EXPECT_EQ(moved.pop(), 2);
    EXPECT_TRUE(moved.empty());

    // Original should be empty and safe to use
    EXPECT_TRUE(original.empty());
    original.push(3);
    EXPECT_EQ(original.pop(), 3);
}

TEST(QueueTest, MoveAssignmentOperator) {
    Queue<int> original;
    original.push(10);
    original.push(20);

    Queue<int> moved;
    moved.push(99); // Some initial state
    moved = std::move(original);

    EXPECT_EQ(moved.pop(), 10);
    EXPECT_EQ(moved.pop(), 20);
    EXPECT_TRUE(moved.empty());

    // Original should be empty
    EXPECT_TRUE(original.empty());
}

TEST(QueueTest, Serialization) {
    Queue<std::string> q;
    q.push("a");
    q.push("b");
    q.push("c");
    // Serializes from head to tail
    EXPECT_EQ(q.serialize(), "a b c");
}

TEST(QueueTest, SerializationEmpty) {
    Queue<int> q;
    EXPECT_EQ(q.serialize(), "");
}

TEST(QueueTest, Deserialization) {
    Queue<std::string> q;
    q.deserialize("head middle tail");
    
    ASSERT_FALSE(q.empty());
    EXPECT_EQ(q.pop(), "head");
    EXPECT_EQ(q.pop(), "middle");
    EXPECT_EQ(q.pop(), "tail");
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, DeserializationOverwrites) {
    Queue<int> q;
    q.push(99);
    q.deserialize("1 2 3");
    
    ASSERT_FALSE(q.empty());
    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
}
