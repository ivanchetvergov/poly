cmake_minimum_required(VERSION 3.16)
project(graph_lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_BINARY_DIR}/common)

add_executable(${PROJECT_NAME}
    src/EulerianCycle.cc
    src/HamiltonianCycle.cc
    src/TSPSolver.cc
    main.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE graph_common)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
