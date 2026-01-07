cmake_minimum_required(VERSION 3.16)
project(graph_main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(lab3)
add_subdirectory(lab1)
add_subdirectory(lab5)

add_executable(${PROJECT_NAME}
    main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE graph_common graph_flow graph_lab1_lib graph_lab5_lib)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
