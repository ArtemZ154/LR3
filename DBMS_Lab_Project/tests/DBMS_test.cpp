#include "gtest/gtest.h"
#include "../DBMS.h"

class DBMSTest : public ::testing::Test {
protected:
    DBMS dbms;

    void SetUp() override {
        dbms.clear();
    }
};

TEST_F(DBMSTest, EmptyCommand) {
    EXPECT_EQ(dbms.execute({}), "Error: Empty command.");
}

TEST_F(DBMSTest, UnknownCommand) {
    EXPECT_EQ(dbms.execute({"UNKNOWN_CMD"}), "Error: Unknown command 'UNKNOWN_CMD'");
}

// --- Array Tests ---
TEST_F(DBMSTest, ArrayCommands) {
    EXPECT_EQ(dbms.execute({"MPUSH", "my_array", "val1", "val2"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "0"}), "-> val1");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "1"}), "-> val2");
    EXPECT_EQ(dbms.execute({"MSET", "my_array", "1", "new_val2"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "1"}), "-> new_val2");
    EXPECT_EQ(dbms.execute({"MINSERT", "my_array", "1", "val1.5"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "1"}), "-> val1.5");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "2"}), "-> new_val2");
    EXPECT_EQ(dbms.execute({"MDEL", "my_array", "0"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "0"}), "-> val1.5");
}

TEST_F(DBMSTest, ArrayErrorHandling) {
    EXPECT_EQ(dbms.execute({"MPUSH", "my_array"}), "Error: MPUSH requires at least one value.");
    EXPECT_EQ(dbms.execute({"MGET", "non_existent_array", "0"}), "Error: Not found (or out of range).");
    EXPECT_EQ(dbms.execute({"MGET", "my_array", "0"}), "Error: Not found (or out of range).");
}

// --- Stack Tests ---
TEST_F(DBMSTest, StackCommands) {
    EXPECT_EQ(dbms.execute({"SEMPTY", "my_stack"}), "Error: Not found (or out of range).");
    EXPECT_EQ(dbms.execute({"SPUSH", "my_stack", "item1"}), "-> OK");
    EXPECT_EQ(dbms.execute({"SEMPTY", "my_stack"}), "-> FALSE");
    EXPECT_EQ(dbms.execute({"SPUSH", "my_stack", "item2"}), "-> OK");
    EXPECT_EQ(dbms.execute({"SPOP", "my_stack"}), "-> item2");
    EXPECT_EQ(dbms.execute({"SPOP", "my_stack"}), "-> item1");
    EXPECT_EQ(dbms.execute({"SEMPTY", "my_stack"}), "-> TRUE");
}

// --- Queue Tests ---
TEST_F(DBMSTest, QueueCommands) {
    EXPECT_EQ(dbms.execute({"QEMPTY", "my_queue"}), "Error: Not found (or out of range).");
    EXPECT_EQ(dbms.execute({"QPUSH", "my_queue", "item1"}), "-> OK");
    EXPECT_EQ(dbms.execute({"QEMPTY", "my_queue"}), "-> FALSE");
    EXPECT_EQ(dbms.execute({"QPUSH", "my_queue", "item2"}), "-> OK");
    EXPECT_EQ(dbms.execute({"QPOP", "my_queue"}), "-> item1");
    EXPECT_EQ(dbms.execute({"QPOP", "my_queue"}), "-> item2");
    EXPECT_EQ(dbms.execute({"QEMPTY", "my_queue"}), "-> TRUE");
}

// --- Singly Linked List Tests ---
TEST_F(DBMSTest, SinglyLinkedListCommands) {
    EXPECT_EQ(dbms.execute({"FPUSHF", "sll", "a"}), "-> OK");
    EXPECT_EQ(dbms.execute({"FPUSHB", "sll", "c"}), "-> OK");
    EXPECT_EQ(dbms.execute({"FINSB", "sll", "c", "b"}), "-> OK"); // a, b, c
    EXPECT_EQ(dbms.execute({"FFIND", "sll", "b"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"FPOPF", "sll"}), "-> a");
    EXPECT_EQ(dbms.execute({"FDELV", "sll", "c"}), "-> OK");
    EXPECT_EQ(dbms.execute({"FFIND", "sll", "c"}), "-> FALSE");
}

// --- Doubly Linked List Tests ---
TEST_F(DBMSTest, DoublyLinkedListCommands) {
    EXPECT_EQ(dbms.execute({"LPUSHF", "dll", "b"}), "-> OK");
    EXPECT_EQ(dbms.execute({"LPUSHF", "dll", "a"}), "-> OK");
    EXPECT_EQ(dbms.execute({"LPUSHB", "dll", "c"}), "-> OK"); // a, b, c
    EXPECT_EQ(dbms.execute({"LFIND", "dll", "b"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"LPOPB", "dll"}), "-> c");
    EXPECT_EQ(dbms.execute({"LPOPF", "dll"}), "-> a");
    EXPECT_EQ(dbms.execute({"LDELV", "dll", "b"}), "-> OK");
    EXPECT_EQ(dbms.execute({"LFIND", "dll", "b"}), "-> FALSE");
}

// --- Set Tests ---
TEST_F(DBMSTest, SetCommands) {
    EXPECT_EQ(dbms.execute({"SETADD", "my_set", "apple", "banana"}), "-> OK");
    EXPECT_EQ(dbms.execute({"SET_AT", "my_set", "apple"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"SET_AT", "my_set", "cherry"}), "-> FALSE");
    EXPECT_EQ(dbms.execute({"SETDEL", "my_set", "apple"}), "-> OK");
    EXPECT_EQ(dbms.execute({"SET_AT", "my_set", "apple"}), "-> FALSE");
}

// --- Tree Tests ---
TEST_F(DBMSTest, TreeCommands) {
    EXPECT_EQ(dbms.execute({"TINSERT", "my_tree", "10"}), "-> OK");
    EXPECT_EQ(dbms.execute({"TINSERT", "my_tree", "5"}), "-> OK");
    EXPECT_EQ(dbms.execute({"TGET", "my_tree", "5"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"TGET", "my_tree", "15"}), "-> FALSE");
}

// --- Hash Table Chaining Tests ---
TEST_F(DBMSTest, HashTableChainingCommands) {
    EXPECT_EQ(dbms.execute({"CH_PUT", "ht_c", "my_key", "my_value"}), "-> OK");
    EXPECT_EQ(dbms.execute({"CH_GET", "ht_c", "my_key"}), "-> my_value");
    EXPECT_EQ(dbms.execute({"CH_DEL", "ht_c", "my_key"}), "-> OK");
    EXPECT_EQ(dbms.execute({"CH_GET", "ht_c", "my_key"}), "Error: Not found (or out of range).");
}

// --- Hash Table Open Addressing Tests ---
TEST_F(DBMSTest, HashTableOpenAddrCommands) {
    EXPECT_EQ(dbms.execute({"OA_PUT", "ht_oa", "my_key", "my_value"}), "-> OK");
    EXPECT_EQ(dbms.execute({"OA_GET", "ht_oa", "my_key"}), "-> my_value");
    EXPECT_EQ(dbms.execute({"OA_DEL", "ht_oa", "my_key"}), "-> OK");
    EXPECT_EQ(dbms.execute({"OA_GET", "ht_oa", "my_key"}), "Error: Not found (or out of range).");
}

// --- LFU Cache Tests ---
TEST_F(DBMSTest, LFUCacheCommands) {
    EXPECT_EQ(dbms.execute({"LFU_INIT", "lfu", "2"}), "-> OK");
    EXPECT_EQ(dbms.execute({"LFU_SET", "lfu", "k1", "v1"}), "-> OK");
    EXPECT_EQ(dbms.execute({"LFU_SET", "lfu", "k2", "v2"}), "-> OK");
    EXPECT_EQ(dbms.execute({"LFU_GET", "lfu", "k1"}), "-> v1");
    EXPECT_EQ(dbms.execute({"LFU_SET", "lfu", "k3", "v3"}), "-> OK"); // k2 should be evicted
    EXPECT_EQ(dbms.execute({"LFU_GET", "lfu", "k2"}), "-> -1");
    EXPECT_EQ(dbms.execute({"LFU_GET", "lfu", "k3"}), "-> v3");
}

// --- Special Task Tests ---

TEST_F(DBMSTest, AsteroidCollision) {
    dbms.execute({"MPUSH", "asteroids_in", "5", "10", "-5"});
    EXPECT_EQ(dbms.execute({"ASTEROID_COLLIDE", "asteroids_in", "asteroids_out"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "asteroids_out", "0"}), "-> 5");
    EXPECT_EQ(dbms.execute({"MGET", "asteroids_out", "1"}), "-> 10");

    dbms.clear();
    dbms.execute({"MPUSH", "asteroids_in", "8", "-8"});
    EXPECT_EQ(dbms.execute({"ASTEROID_COLLIDE", "asteroids_in", "asteroids_out"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "asteroids_out", "0"}), "Error: Not found (or out of range).");
}

TEST_F(DBMSTest, PartitionMinDiff) {
    dbms.execute({"SETADD", "nums", "1", "6", "11", "5"});
    EXPECT_EQ(dbms.execute({"PARTITION_MIN_DIFF", "nums", "s1", "s2"}), "-> Difference: 1");
    // Total sum is 23. Target is 11. s1 should be {11} or {5,6}. s2 is the rest.
    // Let's check one possibility
    EXPECT_EQ(dbms.execute({"SET_AT", "s1", "11"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"SET_AT", "s2", "1"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"SET_AT", "s2", "6"}), "-> TRUE");
    EXPECT_EQ(dbms.execute({"SET_AT", "s2", "5"}), "-> TRUE");
}

TEST_F(DBMSTest, FindSumSubarray) {
    dbms.execute({"MPUSH", "arr_in", "1", "4", "20", "3", "10", "5"});
    EXPECT_EQ(dbms.execute({"FIND_SUM_SUBARRAY", "arr_in", "33", "arr_out"}), "-> OK");
    EXPECT_EQ(dbms.execute({"MGET", "arr_out", "0"}), "-> {20, 3, 10}");
}

TEST_F(DBMSTest, BstAddDepths) {
    dbms.execute({"MPUSH", "bst_in", "8", "3", "10", "1", "6", "14"});
    EXPECT_EQ(dbms.execute({"BST_ADD_DEPTHS", "bst_in", "bst_out"}), "-> OK");
    // Depths: 8->1, 3->2, 10->2, 1->3, 6->3, 14->3
    EXPECT_EQ(dbms.execute({"MGET", "bst_out", "0"}), "-> 1");
    EXPECT_EQ(dbms.execute({"MGET", "bst_out", "1"}), "-> 2");
    EXPECT_EQ(dbms.execute({"MGET", "bst_out", "2"}), "-> 2");
    EXPECT_EQ(dbms.execute({"MGET", "bst_out", "3"}), "-> 3");
    EXPECT_EQ(dbms.execute({"MGET", "bst_out", "4"}), "-> 3");
    EXPECT_EQ(dbms.execute({"MGET", "bst_out", "5"}), "-> 3");
}

TEST_F(DBMSTest, LongestSubstring) {
    EXPECT_EQ(dbms.execute({"LONGEST_SUBSTRING", "abcabcbb"}), "-> 3");
    EXPECT_EQ(dbms.execute({"LONGEST_SUBSTRING", "bbbbb"}), "-> 1");
    EXPECT_EQ(dbms.execute({"LONGEST_SUBSTRING", "pwwkew"}), "-> 3");
}

// --- Serialization/Deserialization Test ---
TEST_F(DBMSTest, SerializationAndLoad) {
    dbms.execute({"MPUSH", "my_array", "a", "b"});
    dbms.execute({"SPUSH", "my_stack", "c"});
    std::string serialized_data = dbms.serializeAll();

    DBMS new_dbms;
    std::stringstream ss(serialized_data);
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream line_ss(line);
        std::string type, name, data;
        line_ss >> type >> name;
        std::getline(line_ss, data);
        if (!data.empty() && data[0] == ' ') {
            data = data.substr(1);
        }
        new_dbms.loadStructure(type, name, data);
    }

    EXPECT_EQ(new_dbms.execute({"MGET", "my_array", "1"}), "-> b");
    EXPECT_EQ(new_dbms.execute({"SPOP", "my_stack"}), "-> c");
}
