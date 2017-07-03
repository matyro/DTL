
# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name

project(DTL CXX)

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


# Set Compiler Version
set (CMAKE_CXX_STANDARD 14)
# Set shared compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -pedantic-errors")


# Add library tuple
#file(GLOB tuple_SRC
#    "${CMAKE_SOURCE_DIR}/include/meta/tuple/*.h"
#    "${CMAKE_SOURCE_DIR}/src/meta/tuple/*.cpp"
#)
#add_library(tuple_lib STATIC "${tuple_SRC}")

# Add library network
file(GLOB network_SRC
    "${CMAKE_SOURCE_DIR}/include/io/network/*.h"
    "${CMAKE_SOURCE_DIR}/src/io/network/*.cpp"
)
add_library(network_lib STATIC "${network_SRC}")


# Find gtest and add it to directory
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add UnitTest
add_executable(unit_test "${CMAKE_SOURCE_DIR}/test/unit_test.cpp")

target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})

find_package (Threads)
target_link_libraries(unit_test "${CMAKE_THREAD_LIBS_INIT}")

target_link_libraries(unit_test network_lib)


