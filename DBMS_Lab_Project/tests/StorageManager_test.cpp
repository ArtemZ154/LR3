#include "gtest/gtest.h"
#include "../StorageManager.h"
#include "../DBMS.h"
#include <fstream>

class StorageManagerTest : public ::testing::Test {
protected:
    const std::string test_filepath = "test_db.dat";
    DBMS dbms;

    void TearDown() override {
        remove(test_filepath.c_str());
    }
};

TEST_F(StorageManagerTest, SaveAndLoad) {
    StorageManager sm(test_filepath);

    // 1. Populate the DBMS
    dbms.execute({"MPUSH", "my_array", "a", "b", "c"});
    dbms.execute({"SPUSH", "my_stack", "s1"});

    // 2. Save to file
    sm.save(dbms);

    // 3. Verify file content
    std::ifstream file(test_filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    EXPECT_NE(content.find("Array my_array a b c"), std::string::npos);
    EXPECT_NE(content.find("Stack my_stack s1"), std::string::npos);
    file.close();

    // 4. Create a new DBMS and load from file
    DBMS new_dbms;
    sm.load(new_dbms);

    // 5. Verify the new DBMS state
    EXPECT_EQ(new_dbms.execute({"MGET", "my_array", "1"}), "-> b");
    EXPECT_EQ(new_dbms.execute({"SPOP", "my_stack"}), "-> s1");
}

TEST_F(StorageManagerTest, LoadFromNonExistentFile) {
    StorageManager sm("non_existent_file.dat");
    DBMS new_dbms;
    new_dbms.execute({"MPUSH", "arr", "val"}); // Give it some initial state
    
    // Load should not throw and should not clear the dbms
    sm.load(new_dbms); 
    
    EXPECT_EQ(new_dbms.execute({"MGET", "arr", "0"}), "-> val");
}
