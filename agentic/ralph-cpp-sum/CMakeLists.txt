cmake_minimum_required(VERSION 3.10)
project(RalphSum VERSION 1.0 LANGUAGES CXX)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest package
find_package(GTest REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})

# Create executable
add_executable(ralph_sum_test test_ralph_sum.cpp)

# Link GTest with main
target_link_libraries(ralph_sum_test ${GTEST_LIBRARIES} gtest_main)
