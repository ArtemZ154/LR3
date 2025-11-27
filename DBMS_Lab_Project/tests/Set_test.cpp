#include "gtest/gtest.h"
#include "../DataStructures/Set.h"
#include <string>
#include <algorithm>
#include <vector>
#include <set> // Using std::set to verify contents easily

// Helper function to get sorted elements from our Set for easy comparison
std::vector<std::string> getSortedElements(const Set<std::string>& set) {
    if (set.empty()) {
        return {};
    }
    std::string* elements = set.getElements();
    std::vector<std::string> vec(elements, elements + set.size());
    delete[] elements;
    std::sort(vec.begin(), vec.end());
    return vec;
}

class SetTest : public ::testing::Test {
protected:
    Set<std::string> set;
};

// --- Core Functionality ---

TEST_F(SetTest, IsInitiallyEmpty) {
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0);
}

TEST_F(SetTest, AddAndContains) {
    set.add("apple");
    set.add("banana");
    EXPECT_FALSE(set.empty());
    EXPECT_EQ(set.size(), 2);
    EXPECT_TRUE(set.contains("apple"));
    EXPECT_TRUE(set.contains("banana"));
    EXPECT_FALSE(set.contains("cherry"));
}

TEST_F(SetTest, AddDuplicatesDoesNotChangeSize) {
    set.add("apple");
    set.add("banana");
    set.add("apple");
    EXPECT_EQ(set.size(), 2);
}

TEST_F(SetTest, Remove) {
    set.add("apple");
    set.add("banana");
    set.remove("apple");
    EXPECT_EQ(set.size(), 1);
    EXPECT_FALSE(set.contains("apple"));
    EXPECT_TRUE(set.contains("banana"));
}

TEST_F(SetTest, RemoveNonExistent) {
    set.add("apple");
    set.remove("banana"); // Should do nothing
    EXPECT_EQ(set.size(), 1);
    EXPECT_TRUE(set.contains("apple"));
}

TEST_F(SetTest, RemoveFromEmptySet) {
    set.remove("apple"); // Should not crash
    EXPECT_TRUE(set.empty());
}

TEST_F(SetTest, Clear) {
    set.add("apple");
    set.add("banana");
    set.clear();
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0);
    EXPECT_FALSE(set.contains("apple"));
}

// --- Tombstone and Rehashing Tests ---

TEST_F(SetTest, AddAfterRemoveReusesTombstone) {
    // This test assumes a small capacity to force probing
    Set<std::string> small_set; // Default capacity is 16, which is fine
    std::string val1 = "a"; // Assume these hash to nearby slots
    std::string val2 = "b"; 
    std::string val3 = "c";

    small_set.add(val1);
    small_set.add(val2);
    small_set.remove(val1); // Creates a tombstone at val1's spot

    // `contains` should correctly probe past the tombstone
    EXPECT_TRUE(small_set.contains(val2));
    EXPECT_FALSE(small_set.contains(val1));

    // `add` should reuse the tombstone slot for val3 if it probes there
    small_set.add(val3);
    EXPECT_EQ(small_set.size(), 2);
    EXPECT_TRUE(small_set.contains(val2));
    EXPECT_TRUE(small_set.contains(val3));
}

TEST_F(SetTest, RehashingWorksCorrectly) {
    // Default capacity 16, load factor 0.6. Rehash should trigger at 16 * 0.6 = 9.6 -> 10th element.
    for (int i = 0; i < 9; ++i) {
        set.add("item" + std::to_string(i));
    }
    EXPECT_EQ(set.size(), 9);

    // This add should trigger rehash
    set.add("item9");
    EXPECT_EQ(set.size(), 10);

    // Add more to be sure
    for (int i = 10; i < 25; ++i) {
        set.add("item" + std::to_string(i));
    }
    EXPECT_EQ(set.size(), 25);

    // Verify all elements are still present
    for (int i = 0; i < 25; ++i) {
        EXPECT_TRUE(set.contains("item" + std::to_string(i)));
    }
}

TEST_F(SetTest, RehashingDiscardsTombstones) {
    for (int i = 0; i < 9; ++i) {
        set.add("item" + std::to_string(i));
    }
    set.remove("item3");
    set.remove("item5");
    EXPECT_EQ(set.size(), 7);

    // Trigger rehash
    set.add("item9");
    set.add("item10");
    set.add("item11");
    EXPECT_EQ(set.size(), 10);

    // After rehashing, the tombstones should be gone, but all active elements should be present.
    EXPECT_TRUE(set.contains("item0"));
    EXPECT_FALSE(set.contains("item3"));
    EXPECT_FALSE(set.contains("item5"));
    EXPECT_TRUE(set.contains("item11"));
}

// --- Rule of Five ---

TEST_F(SetTest, CopyConstructor) {
    set.add("a");
    set.add("b");
    Set<std::string> copy(set);
    
    EXPECT_EQ(copy.size(), 2);
    EXPECT_TRUE(copy.contains("a"));
    
    // Modify original, copy should be unaffected
    set.add("c");
    EXPECT_FALSE(copy.contains("c"));
}

TEST_F(SetTest, CopyAssignmentOperator) {
    set.add("a");
    set.add("b");
    Set<std::string> copy;
    copy.add("z");
    
    copy = set; // Assign
    EXPECT_EQ(copy.size(), 2);
    EXPECT_TRUE(copy.contains("a"));
    EXPECT_FALSE(copy.contains("z"));

    // Modify original, copy should be unaffected
    set.remove("a");
    EXPECT_TRUE(copy.contains("a"));
}

TEST_F(SetTest, CopyAssignmentSelfAssign) {
    set.add("a");
    set = set; // Self-assignment should not crash or corrupt
    EXPECT_EQ(set.size(), 1);
    EXPECT_TRUE(set.contains("a"));
}

TEST_F(SetTest, MoveConstructor) {
    set.add("a");
    set.add("b");
    Set<std::string> moved(std::move(set));

    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(moved.contains("a"));
    
    // Original set should be in a valid but empty state
    EXPECT_TRUE(set.empty());
}

TEST_F(SetTest, MoveAssignmentOperator) {
    set.add("a");
    set.add("b");
    Set<std::string> moved;
    moved.add("z");

    moved = std::move(set);
    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(moved.contains("b"));
    EXPECT_FALSE(moved.contains("z"));

    // Original set should be in a valid but empty state
    EXPECT_TRUE(set.empty());
}

// --- Set Operations ---

TEST_F(SetTest, Union) {
    set.add("a");
    set.add("b");
    Set<std::string> other;
    other.add("b");
    other.add("c");

    Set<std::string> result = set.set_union(other);
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(getSortedElements(result), std::vector<std::string>({"a", "b", "c"}));
}

TEST_F(SetTest, UnionWithEmpty) {
    set.add("a");
    Set<std::string> empty_set;
    Set<std::string> result1 = set.set_union(empty_set);
    Set<std::string> result2 = empty_set.set_union(set);

    EXPECT_EQ(getSortedElements(result1), std::vector<std::string>({"a"}));
    EXPECT_EQ(getSortedElements(result2), std::vector<std::string>({"a"}));
}

TEST_F(SetTest, Intersection) {
    set.add("a");
    set.add("b");
    Set<std::string> other;
    other.add("b");
    other.add("c");

    Set<std::string> result = set.set_intersection(other);
    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(result.contains("b"));
}

TEST_F(SetTest, IntersectionDisjoint) {
    set.add("a");
    Set<std::string> other;
    other.add("b");
    Set<std::string> result = set.set_intersection(other);
    EXPECT_TRUE(result.empty());
}

TEST_F(SetTest, Difference) {
    set.add("a");
    set.add("b");
    Set<std::string> other;
    other.add("b");
    other.add("c");

    Set<std::string> result = set.set_difference(other);
    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(result.contains("a"));
}

TEST_F(SetTest, DifferenceFromSelf) {
    set.add("a");
    set.add("b");
    Set<std::string> result = set.set_difference(set);
    EXPECT_TRUE(result.empty());
}

// --- Serialization and `getElements` ---

TEST_F(SetTest, GetElements) {
    set.add("c");
    set.add("a");
    set.add("b");
    EXPECT_EQ(getSortedElements(set), std::vector<std::string>({"a", "b", "c"}));
}

TEST_F(SetTest, GetElementsFromEmpty) {
    std::string* elements = set.getElements();
    EXPECT_EQ(elements, nullptr);
}

TEST_F(SetTest, SerializationAndDeserialization) {
    set.add("zulu");
    set.add("alpha");
    set.add("charlie");
    
    std::string serialized = set.serialize();
    
    // Use std::set to verify content regardless of order
    std::set<std::string> expected_content = {"zulu", "alpha", "charlie"};
    std::stringstream ss(serialized);
    std::string item;
    std::set<std::string> actual_content;
    while(ss >> item) {
        actual_content.insert(item);
    }
    EXPECT_EQ(actual_content, expected_content);

    Set<std::string> newSet;
    newSet.deserialize(serialized);
    
    EXPECT_EQ(newSet.size(), 3);
    EXPECT_TRUE(newSet.contains("zulu"));
    EXPECT_TRUE(newSet.contains("alpha"));
    EXPECT_TRUE(newSet.contains("charlie"));
}

TEST_F(SetTest, SerializeEmpty) {
    EXPECT_EQ(set.serialize(), "");
}

TEST_F(SetTest, DeserializeEmpty) {
    set.deserialize("");
    EXPECT_TRUE(set.empty());
}

TEST_F(SetTest, DeserializeOverwrites) {
    set.add("old_value");
    set.deserialize("new value");
    EXPECT_EQ(set.size(), 2);
    EXPECT_TRUE(set.contains("new"));
    EXPECT_TRUE(set.contains("value"));
    EXPECT_FALSE(set.contains("old_value"));
}
