cmake_minimum_required(VERSION 3.16)
project(graph_main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/common/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lab1/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lab3/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lab5/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lab1
    ${CMAKE_CURRENT_SOURCE_DIR}/lab3
    ${CMAKE_CURRENT_SOURCE_DIR}/lab5
)

add_executable(${PROJECT_NAME}
    main.cc
    lab1/src/ShimbellMethod.cc
    lab1/src/PathCounter.cc
    lab3/src/FlowNetwork.cc
    lab3/src/MaxFlow.cc
    lab3/src/MinCostFlow.cc
    lab5/src/EulerianCycle.cc
    lab5/src/HamiltonianCycle.cc
    lab5/src/TSPSolver.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE graph_common)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
