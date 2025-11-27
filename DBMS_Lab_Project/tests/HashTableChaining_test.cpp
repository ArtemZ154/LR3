#include "gtest/gtest.h"
#include "../DataStructures/HashTableChaining.h"
#include <string>

class HashTableChainingTest : public ::testing::Test {
protected:
    HashTableChaining ht;
};

TEST_F(HashTableChainingTest, PutAndGet) {
    ht.put("key1", "value1");
    ht.put("key2", "value2");
    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("key2"), "value2");
}

TEST_F(HashTableChainingTest, GetNonExistentThrows) {
    EXPECT_THROW(ht.get("nonexistent"), std::out_of_range);
}

TEST_F(HashTableChainingTest, UpdateValue) {
    ht.put("key1", "value1");
    ht.put("key1", "new_value1");
    EXPECT_EQ(ht.get("key1"), "new_value1");
}

TEST_F(HashTableChainingTest, Remove) {
    ht.put("key1", "value1");
    ht.remove("key1");
    EXPECT_THROW(ht.get("key1"), std::out_of_range);
}

TEST_F(HashTableChainingTest, RemoveNonExistent) {
    // Should not throw or crash
    ht.remove("nonexistent");
    SUCCEED();
}

TEST_F(HashTableChainingTest, CollisionHandling) {
    // Assuming default capacity and a simple hash, these might collide.
    // We can't guarantee collision, but we can test that both values are stored.
    ht.put("key1", "value1");
    ht.put("a_different_key_that_might_collide", "value2");
    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("a_different_key_that_might_collide"), "value2");
}

TEST_F(HashTableChainingTest, Serialization) {
    ht.put("k1", "v1");
    ht.put("k2", "v2");
    std::string serialized = ht.serialize();
    // Order is not guaranteed, so check for parts
    EXPECT_NE(serialized.find("k1:v1"), std::string::npos);
    EXPECT_NE(serialized.find("k2:v2"), std::string::npos);
}

TEST_F(HashTableChainingTest, Deserialization) {
    std::string data = "key1:val1 key2:val2 ";
    ht.deserialize(data);
    EXPECT_EQ(ht.get("key1"), "val1");
    EXPECT_EQ(ht.get("key2"), "val2");
}

TEST_F(HashTableChainingTest, SerializeDeserializeEmpty) {
    EXPECT_EQ(ht.serialize(), "");
    ht.deserialize("");
    EXPECT_THROW(ht.get("any"), std::out_of_range);
}
