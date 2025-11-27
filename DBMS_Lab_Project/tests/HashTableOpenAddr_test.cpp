#include "gtest/gtest.h"
#include "../DataStructures/HashTableOpenAddr.h"
#include <string>

class HashTableOpenAddrTest : public ::testing::Test {
protected:
    HashTableOpenAddr ht;
};

TEST_F(HashTableOpenAddrTest, PutAndGet) {
    ht.put("key1", "value1");
    ht.put("key2", "value2");
    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("key2"), "value2");
}

TEST_F(HashTableOpenAddrTest, GetOverload) {
    ht.put("key1", "value1");
    std::string value;
    EXPECT_TRUE(ht.get("key1", value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(ht.get("nonexistent", value));
}

TEST_F(HashTableOpenAddrTest, GetNonExistentThrows) {
    EXPECT_THROW(ht.get("nonexistent"), std::out_of_range);
}

TEST_F(HashTableOpenAddrTest, UpdateValue) {
    ht.put("key1", "value1");
    ht.put("key1", "new_value1");
    EXPECT_EQ(ht.get("key1"), "new_value1");
}

TEST_F(HashTableOpenAddrTest, Remove) {
    ht.put("key1", "value1");
    ht.remove("key1");
    EXPECT_THROW(ht.get("key1"), std::out_of_range);
}

TEST_F(HashTableOpenAddrTest, RemoveAndReinsert) {
    ht.put("key1", "value1");
    ht.remove("key1");
    ht.put("key1", "value2");
    EXPECT_EQ(ht.get("key1"), "value2");
}

TEST_F(HashTableOpenAddrTest, Rehash) {
    // Capacity 16, load factor 0.6 -> rehash at 10 elements
    for (int i = 0; i < 20; ++i) {
        ht.put("key" + std::to_string(i), "value" + std::to_string(i));
    }
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(ht.get("key" + std::to_string(i)), "value" + std::to_string(i));
    }
}

TEST_F(HashTableOpenAddrTest, Clear) {
    ht.put("key1", "value1");
    ht.clear();
    EXPECT_THROW(ht.get("key1"), std::out_of_range);
}

TEST_F(HashTableOpenAddrTest, Serialization) {
    ht.put("k1", "v1");
    ht.put("k2", "v2");
    std::string serialized = ht.serialize();
    // Order is not guaranteed, so check for parts
    EXPECT_NE(serialized.find("k1:v1"), std::string::npos);
    EXPECT_NE(serialized.find("k2:v2"), std::string::npos);
}

TEST_F(HashTableOpenAddrTest, Deserialization) {
    std::string data = "key1:val1 key2:val2 ";
    ht.deserialize(data);
    EXPECT_EQ(ht.get("key1"), "val1");
    EXPECT_EQ(ht.get("key2"), "val2");
}
