#include "gtest/gtest.h"
#include "../DataStructures/LFUCache.h"
#include <string>

class LFUCacheTest : public ::testing::Test {
protected:
    // Инициализация происходит в каждом тесте
};

TEST_F(LFUCacheTest, BasicSetAndGet) {
    LFUCache cache(2);
    cache.set("k1", "v1");
    cache.set("k2", "v2");
    EXPECT_EQ(cache.get("k1"), "v1");
    EXPECT_EQ(cache.get("k2"), "v2");
}

TEST_F(LFUCacheTest, GetUpdatesFrequency) {
    LFUCache cache(2);
    cache.set("k1", "v1");
    cache.set("k2", "v2");
    cache.get("k1"); // k1 freq=2, k2 freq=1
    cache.set("k3", "v3"); // evicts k2
    EXPECT_EQ(cache.get("k1"), "v1");
    EXPECT_EQ(cache.get("k2"), "-1");
    EXPECT_EQ(cache.get("k3"), "v3");
}

TEST_F(LFUCacheTest, EvictionOfLeastFrequentlyUsed) {
    LFUCache cache(2);
    cache.set("k1", "v1"); // k1 freq=1
    cache.set("k2", "v2"); // k2 freq=1
    cache.get("k1");       // k1 freq=2
    cache.get("k1");       // k1 freq=3
    cache.get("k2");       // k2 freq=2
    cache.set("k3", "v3"); // k2 is LRU among the least frequent, but its freq is 2. Wait, this is wrong.
                           // k1 freq=3, k2 freq=2. minFreq is 2. k2 should be evicted.
    EXPECT_EQ(cache.get("k1"), "v1");
    EXPECT_EQ(cache.get("k2"), "-1");
    EXPECT_EQ(cache.get("k3"), "v3");
}

TEST_F(LFUCacheTest, EvictionOfLeastRecentlyUsedAmongSameFrequency) {
    LFUCache cache(2);
    cache.set("k1", "v1"); // k1 freq=1, LRU
    cache.set("k2", "v2"); // k2 freq=1, MRU
    cache.set("k3", "v3"); // evicts k1
    EXPECT_EQ(cache.get("k1"), "-1");
    EXPECT_EQ(cache.get("k2"), "v2");
    EXPECT_EQ(cache.get("k3"), "v3");
}

TEST_F(LFUCacheTest, UpdateValue) {
    LFUCache cache(1);
    cache.set("k1", "v1");
    cache.set("k1", "v1_new");
    EXPECT_EQ(cache.get("k1"), "v1_new");
}

TEST_F(LFUCacheTest, ZeroCapacity) {
    LFUCache cache(0);
    cache.set("k1", "v1");
    EXPECT_EQ(cache.get("k1"), "-1");
}

TEST_F(LFUCacheTest, Serialization) {
    LFUCache cache(3);
    cache.set("a", "1");
    cache.set("b", "2");
    cache.get("a"); // a freq=2, b freq=1
    
    std::string serialized = cache.serialize();
    // capacity a 1 2 b 2 1
    // The order in unordered_map is not guaranteed, so we check for content
    EXPECT_NE(serialized.find("3"), std::string::npos); // capacity
    EXPECT_NE(serialized.find("a 1 2"), std::string::npos);
    EXPECT_NE(serialized.find("b 2 1"), std::string::npos);
}

TEST_F(LFUCacheTest, Deserialization) {
    // capacity key val freq ...
    std::string data = "2 a 10 2 b 20 1";
    LFUCache cache;
    cache.deserialize(data);

    EXPECT_EQ(cache.get("a"), "10"); // get will increase freq of a to 3
    EXPECT_EQ(cache.get("b"), "20"); // get will increase freq of b to 2

    // Now, a has freq 3, b has freq 2. minFreq is 2.
    // Adding a new element should evict b.
    cache.set("c", "30");
    EXPECT_EQ(cache.get("a"), "10");
    EXPECT_EQ(cache.get("b"), "-1");
    EXPECT_EQ(cache.get("c"), "30");
}

TEST_F(LFUCacheTest, MoveConstructorAndAssignment) {
    LFUCache original(2);
    original.set("k1", "v1");
    
    LFUCache moved_constructor = std::move(original);
    EXPECT_EQ(moved_constructor.get("k1"), "v1");
    
    // original is now in a valid but unspecified state. Let's test assignment.
    LFUCache new_original(1);
    new_original.set("k_new", "v_new");
    
    LFUCache moved_assignment;
    moved_assignment = std::move(new_original);
    EXPECT_EQ(moved_assignment.get("k_new"), "v_new");
}
