cmake_minimum_required(VERSION 3.16)
project(graph_main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()

add_subdirectory(common)
add_subdirectory(lab1)
add_subdirectory(lab3)
add_subdirectory(lab5)

add_executable(${PROJECT_NAME}
    main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE graph_common graph_flow graph_lab1_lib graph_lab5_lib)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# Custom target to auto-fix code with clang-tidy and clang-format
find_program(CLANG_TIDY_EXE clang-tidy)
find_program(CLANG_FORMAT_EXE clang-format)

if(CLANG_TIDY_EXE AND CLANG_FORMAT_EXE)
    add_custom_target(fix-code
        COMMAND ${CMAKE_COMMAND} -E echo "Finding source files..."
        COMMAND find ${CMAKE_SOURCE_DIR} -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" | grep -v build | grep -v cmake-build > source_files.txt
        COMMAND ${CMAKE_COMMAND} -E echo "Applying clang-tidy fixes..."
        COMMAND xargs -a source_files.txt -n1 ${CLANG_TIDY_EXE} --fix --quiet || true
        COMMAND ${CMAKE_COMMAND} -E echo "Applying clang-format..."
        COMMAND xargs -a source_files.txt ${CLANG_FORMAT_EXE} -i
        COMMAND ${CMAKE_COMMAND} -E remove source_files.txt
        COMMAND ${CMAKE_COMMAND} -E echo "Code fixes applied. Rebuild with: make"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Auto-fix code style and issues with clang-tidy and clang-format"
        VERBATIM
    )
endif()
