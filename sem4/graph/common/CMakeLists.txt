cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(graph_common STATIC
    src/Graph.cc
    src/GraphGenerator.cc
    src/Visualizer.cc
)

target_include_directories(graph_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(graph_common PRIVATE -Wall -Wextra)
