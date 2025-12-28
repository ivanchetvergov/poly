#include <gtest/gtest.h>
#include "HashTable.h"

using namespace dict;

TEST(HashTableTest, InsertAndSearch) {
    HashTable table(10);
    EXPECT_FALSE(table.search("hello"));
    table.insert("hello");
    EXPECT_TRUE(table.search("hello"));
    table.insert("world");
    EXPECT_TRUE(table.search("world"));
    table.insert("hello");
    EXPECT_TRUE(table.search("hello"));
}

TEST(HashTableTest, Remove) {
    HashTable table(10);
    table.insert("a");
    table.insert("b");
    table.insert("c");
    EXPECT_TRUE(table.search("b"));
    EXPECT_TRUE(table.remove("b"));
    EXPECT_FALSE(table.search("b"));
    EXPECT_FALSE(table.remove("b"));
}

TEST(HashTableTest, Clear) {
    HashTable table(10);
    table.insert("x");
    table.insert("y");
    table.insert("z");
    EXPECT_EQ(table.size(), 3);
    table.clear();
    EXPECT_EQ(table.size(), 0);
    EXPECT_FALSE(table.search("x"));
    EXPECT_FALSE(table.search("y"));
    EXPECT_FALSE(table.search("z"));
}

TEST(HashTableTest, CollisionHandling) {
    HashTable table(2);
    table.insert("a");
    table.insert("b");
    table.insert("c");
    EXPECT_TRUE(table.search("a"));
    EXPECT_TRUE(table.search("b"));
    EXPECT_TRUE(table.search("c"));
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}