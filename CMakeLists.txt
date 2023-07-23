cmake_minimum_required(VERSION 3.16)

project(QtExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

add_executable(QtExample
    "src/main.cpp"
    "src/grouping_model.cpp"
    "src/model.cpp"
    
    "include/QtExamples/model.h"
    "include/QtExamples/grouping_model.h"

    "src/grouping_model_test.cpp"
    "include/QtExamples/grouping_model_test.h"
)

target_link_libraries(QtExample PRIVATE Qt5::Widgets Qt5::Core)
target_include_directories(QtExample PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")