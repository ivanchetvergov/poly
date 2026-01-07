cmake_minimum_required(VERSION 3.16)
project(graph_lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT TARGET graph_common)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_BINARY_DIR}/common)
endif()

if(NOT TARGET graph_flow)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lab3 ${CMAKE_CURRENT_BINARY_DIR}/lab3)
endif()

add_library(graph_lab5_lib STATIC
    src/EulerianCycle.cc
    src/HamiltonianCycle.cc
    src/TSPSolver.cc
)
target_include_directories(graph_lab5_lib PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(graph_lab5_lib PUBLIC graph_common)
target_compile_options(graph_lab5_lib PRIVATE -Wall -Wextra)

add_executable(graph_lab5
    main.cc
)

target_include_directories(graph_lab5 PRIVATE include ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(graph_lab5 PRIVATE graph_common graph_lab5_lib graph_flow)
target_compile_options(graph_lab5 PRIVATE -Wall -Wextra)
