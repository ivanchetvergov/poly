cmake_minimum_required(VERSION 3.16)

project(lab7_2 VERSION 1.0 LANGUAGES CXX)

# включаем автоматику qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cc
    ui/src/MainWindow.cc
    ui/src/FiguresScene.cc
    core/src/RectItem.cc
    core/src/TriagItem.cc
    core/src/EllipseItem.cc
)

set(HEADERS
    ui/include/MainWindow.h
    ui/include/FiguresScene.h
    core/include/RectItem.h
    core/include/TriagItem.h
    core/include/EllipseItem.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/ui/include
    ${PROJECT_SOURCE_DIR}/core/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
