cmake_minimum_required(VERSION 3.16)
project(postage VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(postage
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/AddAccountDialog.h
    src/AddAccountDialog.cpp
)

target_link_libraries(postage PRIVATE Qt6::Widgets)

target_include_directories(postage PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
