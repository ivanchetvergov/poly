cmake_minimum_required(VERSION 3.16)
project(graph_lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(graph_flow STATIC
    src/FlowNetwork.cc
    src/MaxFlow.cc
    src/MinCostFlow.cc
    src/Runner.cc
)
target_include_directories(graph_flow PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR}/../common/include)
target_link_libraries(graph_flow PUBLIC graph_common)
target_compile_options(graph_flow PRIVATE -Wall -Wextra)
