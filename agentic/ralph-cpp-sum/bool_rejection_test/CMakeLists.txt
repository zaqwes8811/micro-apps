cmake_minimum_required(VERSION 3.10)
project(bool_rejection_test LANGUAGES CXX)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest package
find_package(GTest REQUIRED)

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR}/..)
include_directories(${GTEST_INCLUDE_DIRS})

# Create executable for compile-time bool rejection test
# This test must FAIL to compile - it should not build
add_executable(bool_rejection_test bool_rejection_test.cpp)
target_link_libraries(bool_rejection_test ${GTEST_LIBRARIES} gtest_main)
