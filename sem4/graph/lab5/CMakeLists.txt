cmake_minimum_required(VERSION 3.16)
project(graph_lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(graph_lab5_lib STATIC
    src/EulerianCycle.cc
    src/CutSystem.cc
    src/Runner.cc
)
target_include_directories(graph_lab5_lib PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
target_link_libraries(graph_lab5_lib PUBLIC graph_common graph_lab4_lib)
target_compile_options(graph_lab5_lib PRIVATE -Wall -Wextra)
