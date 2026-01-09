cmake_minimum_required(VERSION 3.16)
project(graph_lab4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(graph_lab4_lib STATIC
    src/GraphCombinatorics.cc
    src/KirchhoffTheorem.cc
    src/Runner.cc
)
target_include_directories(graph_lab4_lib PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
target_link_libraries(graph_lab4_lib PUBLIC graph_common)
target_compile_options(graph_lab4_lib PRIVATE -Wall -Wextra)
