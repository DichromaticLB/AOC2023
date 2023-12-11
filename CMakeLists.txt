cmake_minimum_required(VERSION 3.25)
project(AOC2023 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
include_directories(include)

add_executable(day1 day_01.cpp util.cpp)

set_target_properties(day1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
