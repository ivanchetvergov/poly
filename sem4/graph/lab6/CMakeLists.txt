cmake_minimum_required(VERSION 3.16)
project(graph_lab6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../common/include)

add_library(graph_lab6_lib STATIC
    src/HashTable.cc
    src/RBTree.cc
    src/TextGenerator.cc
    src/Runner.cc
)
target_include_directories(graph_lab6_lib PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
target_link_libraries(graph_lab6_lib PUBLIC graph_common PRIVATE graph_flow)
target_compile_options(graph_lab6_lib PRIVATE -Wall -Wextra)
