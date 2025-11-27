#include "gtest/gtest.h"
#include "../DataStructures/FullBinaryTree.h"
#include <string>

class FullBinaryTreeTest : public ::testing::Test {
protected:
    FullBinaryTree<std::string> tree;
};

TEST_F(FullBinaryTreeTest, IsInitiallyEmptyAndFull) {
    EXPECT_TRUE(tree.isFull());
    EXPECT_EQ(tree.serialize(), "");
}

TEST_F(FullBinaryTreeTest, InsertAndFind) {
    tree.insert("10");
    tree.insert("20");
    tree.insert("30");
    EXPECT_TRUE(tree.find("10"));
    EXPECT_TRUE(tree.find("20"));
    EXPECT_TRUE(tree.find("30"));
    EXPECT_FALSE(tree.find("40"));
}

TEST_F(FullBinaryTreeTest, IsFull) {
    // 1 node - full
    tree.insert("A");
    EXPECT_TRUE(tree.isFull());

    // 2 nodes - not full
    tree.insert("B");
    EXPECT_FALSE(tree.isFull());

    // 3 nodes - full
    tree.insert("C");
    EXPECT_TRUE(tree.isFull());

    // 4 nodes - not full
    tree.insert("D");
    EXPECT_FALSE(tree.isFull());
}

TEST_F(FullBinaryTreeTest, Serialization) {
    tree.insert("F");
    tree.insert("B");
    tree.insert("G");
    tree.insert("A");
    tree.insert("D");
    tree.insert("I");
    tree.insert("H");
    // Expected structure:
    // F
    // .B
    // ..A
    // ..D
    // .G
    // ..I
    // ..H
    std::string expected = "F|.B|..A|..D|.G|..I|..H";
    EXPECT_EQ(tree.serialize(), expected);
}

TEST_F(FullBinaryTreeTest, Deserialization) {
    std::string data = "10|.5|..2|..7|.15|..12|..20";
    tree.deserialize(data);

    EXPECT_TRUE(tree.find("10"));
    EXPECT_TRUE(tree.find("5"));
    EXPECT_TRUE(tree.find("2"));
    EXPECT_TRUE(tree.find("7"));
    EXPECT_TRUE(tree.find("15"));
    EXPECT_TRUE(tree.find("12"));
    EXPECT_TRUE(tree.find("20"));
    EXPECT_FALSE(tree.find("99"));

    EXPECT_TRUE(tree.isFull());
    EXPECT_EQ(tree.serialize(), data);
}

TEST_F(FullBinaryTreeTest, SerializeDeserializeEmpty) {
    EXPECT_EQ(tree.serialize(), "");
    tree.deserialize("");
    EXPECT_EQ(tree.serialize(), "");
}

TEST_F(FullBinaryTreeTest, MoveConstructor) {
    tree.insert("1");
    tree.insert("2");
    FullBinaryTree<std::string> moved_tree = std::move(tree);
    EXPECT_TRUE(moved_tree.find("1"));
    EXPECT_TRUE(moved_tree.find("2"));
    EXPECT_FALSE(tree.find("1")); // Original tree should be empty
}

TEST_F(FullBinaryTreeTest, MoveAssignment) {
    tree.insert("1");
    tree.insert("2");
    FullBinaryTree<std::string> moved_tree;
    moved_tree.insert("99");
    moved_tree = std::move(tree);
    EXPECT_TRUE(moved_tree.find("1"));
    EXPECT_TRUE(moved_tree.find("2"));
    EXPECT_FALSE(moved_tree.find("99"));
    EXPECT_FALSE(tree.find("1")); // Original tree should be empty
}
