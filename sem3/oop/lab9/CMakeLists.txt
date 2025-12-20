cmake_minimum_required(VERSION 3.16)
project(lab9 VERSION 1.0.0 LANGUAGES CXX)

option(ENABLE_ASAN "Enable AddressSanitizer for memory error detection" OFF)

if (ENABLE_ASAN)
    message(STATUS "AddressSanitizer (ASan) enabled.")
    
    # проверяем компилятор 
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Флаги компиляции:
        # -fsanitize=address: Включает сам санитайзер.
        # -g: Включает отладочную информацию.
        # -O0: Отключает оптимизацию (критично для точных отчетов ASan).
        # -fno-omit-frame-pointer: Для лучшей трассировки стека.
        set(ASAN_FLAGS "-fsanitize=address -g -O0 -fno-omit-frame-pointer")
        
        # Применяем флаги к компилятору и линкеру
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        
    else()
        message(WARNING "AddressSanitizer is requested, but compiler is not GCC or Clang. ASan may not work.")
    endif()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)
find_package(Boost 1.70 REQUIRED)

set(SOURCES
    main.cc
    src/domain/shape.cc
    src/domain/shape_types.cc
    src/domain/shape_factory.cc
    src/infrastructure/database_manager.cc
    src/infrastructure/shape_repository.cc
    src/presentation/main_window.cc
    src/presentation/graphics_view.cc
    src/presentation/shape_delegate.cc
    src/presentation/shape_model.cc
)

set(HEADERS
    include/domain/shape.h
    include/domain/shape_types.h
    include/domain/shape_factory.h
    include/infrastructure/database_manager.h
    include/infrastructure/shape_repository.h
    include/presentation/main_window.h
    include/presentation/graphics_view.h
    include/presentation/shape_delegate.h
    include/presentation/shape_model.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    ${Boost_LIBRARIES}
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()