#include "RBTree.h"

#include <gtest/gtest.h>

using namespace dict;

TEST(RBTreeTest, InsertAndSearch) {
    RBTree tree;
    EXPECT_FALSE(tree.search("hello"));

    tree.insert("hello");
    EXPECT_TRUE(tree.search("hello"));

    tree.insert("world");
    EXPECT_TRUE(tree.search("world"));

    tree.insert("hello");
    EXPECT_TRUE(tree.search("hello"));
}

TEST(RBTreeTest, Remove) {
    RBTree tree;
    tree.insert("alpha");
    tree.insert("beta");
    tree.insert("gamma");

    EXPECT_TRUE(tree.search("beta"));
    EXPECT_TRUE(tree.remove("beta"));
    EXPECT_FALSE(tree.search("beta"));
    EXPECT_FALSE(tree.remove("beta"));
}

TEST(RBTreeTest, Clear) {
    RBTree tree;
    tree.insert("x");
    tree.insert("y");
    tree.insert("z");

    tree.clear();
    EXPECT_FALSE(tree.search("x"));
    EXPECT_FALSE(tree.search("y"));
    EXPECT_FALSE(tree.search("z"));
}

TEST(RBTreeTest, MultipleInsertions) {
    RBTree tree;
    tree.insert("dog");
    tree.insert("cat");
    tree.insert("bird");
    tree.insert("fish");
    tree.insert("elephant");

    EXPECT_TRUE(tree.search("dog"));
    EXPECT_TRUE(tree.search("cat"));
    EXPECT_TRUE(tree.search("bird"));
    EXPECT_TRUE(tree.search("fish"));
    EXPECT_TRUE(tree.search("elephant"));
    EXPECT_FALSE(tree.search("lion"));
}

TEST(RBTreeTest, RemoveRoot) {
    RBTree tree;
    tree.insert("root");
    EXPECT_TRUE(tree.search("root"));
    EXPECT_TRUE(tree.remove("root"));
    EXPECT_FALSE(tree.search("root"));
}

TEST(RBTreeTest, FileOperations) {
    RBTree tree;
    tree.insert("apple");
    tree.insert("banana");
    tree.insert("cherry");

    EXPECT_TRUE(tree.saveToFile("test_rbtree.txt"));

    RBTree tree2;
    EXPECT_TRUE(tree2.loadFromFile("test_rbtree.txt"));
    EXPECT_TRUE(tree2.search("apple"));
    EXPECT_TRUE(tree2.search("banana"));
    EXPECT_TRUE(tree2.search("cherry"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
