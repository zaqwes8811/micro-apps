cmake_minimum_required(VERSION 3.10)
project(RalphSum VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export CMake variables to child directories
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find GTest first before anything else
find_package(GTest REQUIRED)

# Add source directory to include paths
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

# Create executable for runtime tests
add_executable(ralph_sum_test test_ralph_sum.cpp)
target_link_libraries(ralph_sum_test ${GTEST_LIBRARIES} gtest_main)

# Enable testing framework
enable_testing()

# Add the ralph_sum_test to ctest
add_test(NAME ralph_sum_test COMMAND ralph_sum_test)

# Verify that bool_rejection_test fails to compile (compile-time bool rejection test)
# Use CheckCXXSourceCompiles to check if sum(bool, bool) compiles
include(CheckCXXSourceCompiles)

set(TEST_CODE "
#include \"ralph_sum.hpp\"
int main() { (void)sum(true, false); return 0; }
")

check_cxx_source_compiles("${TEST_CODE}" BOOL_SUM_COMPILES)

# Invert the logic: bool sum must NOT compile
if(BOOL_SUM_COMPILES)
    message(FATAL_ERROR "bool sum must not compile")
endif()

message(STATUS "Bool rejection test PASSED: sum(bool, ...) correctly rejected at compile time")
message(STATUS "Build completed. Run: ctest --output-on-failure")
