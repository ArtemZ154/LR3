#include "gtest/gtest.h"
#include "../DataStructures/Set.h"
#include <vector>
#include <algorithm>
#include <set>

// Helper function to get sorted elements from our Set for easy comparison
std::vector<int> getSortedElements(const Set<int>& set) {
    if (set.empty()) {
        return {};
    }
    int* elements = set.getElements();
    std::vector<int> vec(elements, elements + set.size());
    delete[] elements;
    std::sort(vec.begin(), vec.end());
    return vec;
}

class SetIntTest : public ::testing::Test {
protected:
    Set<int> set;
};

TEST_F(SetIntTest, IsInitiallyEmpty) {
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0);
}

TEST_F(SetIntTest, AddAndContains) {
    set.add(10);
    set.add(20);
    EXPECT_FALSE(set.empty());
    EXPECT_EQ(set.size(), 2);
    EXPECT_TRUE(set.contains(10));
    EXPECT_TRUE(set.contains(20));
    EXPECT_FALSE(set.contains(30));
}

TEST_F(SetIntTest, AddDuplicatesDoesNotChangeSize) {
    set.add(10);
    set.add(20);
    set.add(10);
    EXPECT_EQ(set.size(), 2);
}

TEST_F(SetIntTest, Remove) {
    set.add(10);
    set.add(20);
    set.remove(10);
    EXPECT_EQ(set.size(), 1);
    EXPECT_FALSE(set.contains(10));
    EXPECT_TRUE(set.contains(20));
}

TEST_F(SetIntTest, RehashingWorksCorrectly) {
    for (int i = 0; i < 25; ++i) {
        set.add(i * 10);
    }
    EXPECT_EQ(set.size(), 25);
    for (int i = 0; i < 25; ++i) {
        EXPECT_TRUE(set.contains(i * 10));
    }
}

TEST_F(SetIntTest, Union) {
    set.add(1);
    set.add(2);
    Set<int> other;
    other.add(2);
    other.add(3);

    Set<int> result = set.set_union(other);
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(getSortedElements(result), std::vector<int>({1, 2, 3}));
}

TEST_F(SetIntTest, Intersection) {
    set.add(1);
    set.add(2);
    Set<int> other;
    other.add(2);
    other.add(3);

    Set<int> result = set.set_intersection(other);
    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(result.contains(2));
}

TEST_F(SetIntTest, Difference) {
    set.add(1);
    set.add(2);
    Set<int> other;
    other.add(2);
    other.add(3);

    Set<int> result = set.set_difference(other);
    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(result.contains(1));
}
