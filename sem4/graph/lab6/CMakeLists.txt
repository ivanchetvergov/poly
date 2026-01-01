cmake_minimum_required(VERSION 3.10)
project(lab6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги для корректной работы с UTF-8
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(HashTable STATIC
    src/HashTable.cc
)

add_library(RBTree STATIC
    src/RBTree.cc
)

add_library(TextGenerator STATIC
    src/TextGenerator.cc
)

add_executable(lab6_main
    main.cc
)

target_link_libraries(lab6_main HashTable RBTree TextGenerator)
