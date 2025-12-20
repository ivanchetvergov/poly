cmake_minimum_required(VERSION 3.16)
project(lab2_boolean_logic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab2
    src/main.cpp
    src/bdd.cpp
    src/help.cpp
    src/ui.cpp
    src/zheg.cpp
)

target_include_directories(lab2 PRIVATE include)

if (MSVC)
    target_compile_options(lab2 PRIVATE /W4 /permissive-)
else()
    target_compile_options(lab2 PRIVATE -Wall -Wextra -Wpedantic)
endif()
