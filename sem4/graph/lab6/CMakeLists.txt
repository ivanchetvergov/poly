cmake_minimum_required(VERSION 3.16)
project(graph_lab6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../common/include)

add_subdirectory(../lab3 lab3_build)
add_subdirectory(../common common_build)

add_library(HashTable STATIC
    src/HashTable.cc
)

add_library(RBTree STATIC
    src/RBTree.cc
)

add_library(TextGenerator STATIC
    src/TextGenerator.cc
)

add_library(Runner STATIC
    src/Runner.cc
)

add_executable(${PROJECT_NAME}
    main.cc
)

target_link_libraries(${PROJECT_NAME} HashTable RBTree TextGenerator Runner graph_common graph_flow)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
