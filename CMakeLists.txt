
# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name

project(DTL)

set (DTL_VERSION_MAJOR 1)
set (DTL_VERSION_MINOR 0)
set (DTL_VERSION_PATCH 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/config/Config.h.in"
    "${PROJECT_BINARY_DIR}/Config.h"
)
include_directories("${PROJECT_BINARY_DIR}")



file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/obj")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

set(SOURCES ${CMAKE_SOURCE_DIR}/src/)

# The following folder will be included
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/test")




# Add UnitTest
add_executable(unit_test "${CMAKE_SOURCE_DIR}/test/unit_test.cpp")

find_library(GTEST_LIB gtest)
target_link_libraries(unit_test "${GTEST_LIB}")

find_path(GTEST_INCLUDE gtest)
message(STATUS "Add directory ${GTEST_INCLUDE}")
include_directories(unit_test "${GTEST_INCLUDE}")

