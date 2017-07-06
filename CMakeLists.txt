
# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.0)

# Project's name

project(DTL CXX)
ENABLE_LANGUAGE(C)

set (DTL_VERSION_MAJOR 1)
set (DTL_VERSION_MINOR 0)
set (DTL_VERSION_PATCH 0)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/../config/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)




file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/obj")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

set(SOURCES ${CMAKE_SOURCE_DIR}/src/)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/test")


# Set Compiler Version
set (CMAKE_CXX_STANDARD 14)
# Set shared compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -pedantic-errors")


# Check for header
INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES(arpa/inet.h HAVE_ARPA_INET_H)
CHECK_INCLUDE_FILES(netdb.h HAVE_NETDB_H)
CHECK_INCLUDE_FILES(netinet/in.h HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES(sys/socket.h HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)

# Check for functions
INCLUDE (CheckFunctionExists)
CHECK_FUNCTION_EXISTS(gethostbyname HAVE_GETHOSTBYNAME)
CHECK_FUNCTION_EXISTS(inet_ntoa HAVE_INET_NTOA)
CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET)
CHECK_FUNCTION_EXISTS(select HAVE_SELECT)
CHECK_FUNCTION_EXISTS(socket HAVE_SOCKET)
CHECK_FUNCTION_EXISTS(strerror HAVE_STRERROR)



# Add library network
file(GLOB network_SRC
    "${PROJECT_SOURCE_DIR}/include/io/network/*.h"
    "${PROJECT_SOURCE_DIR}/src/io/network/*.cpp"
)
add_library(network_lib STATIC "${network_SRC}")


# Find gtest and add it to directory
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add UnitTest
add_executable(unit_test "${PROJECT_SOURCE_DIR}/test/unit_test.cpp")

target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})

find_package (Threads)
target_link_libraries(unit_test "${CMAKE_THREAD_LIBS_INIT}")

target_link_libraries(unit_test network_lib)
