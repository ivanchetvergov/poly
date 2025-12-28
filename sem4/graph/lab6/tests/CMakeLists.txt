cmake_minimum_required(VERSION 3.10)
project(lab6_hash_table_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../include)

add_library(HashTable STATIC
    ../src/HashTable.cc
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Test_HashTable
    Test_HashTable.cc
)

target_link_libraries(Test_HashTable HashTable GTest::GTest GTest::Main pthread)

enable_testing()
add_test(NAME HashTableTests COMMAND Test_HashTable)
