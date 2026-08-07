cmake_minimum_required(VERSION 3.10)
project(RalphSum VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

# Create executable for runtime tests
add_executable(ralph_sum_test test_ralph_sum.cpp)
target_link_libraries(ralph_sum_test ${GTEST_LIBRARIES} gtest_main)

# Add the ralph_sum_test to ctest
add_test(NAME ralph_sum_test COMMAND ralph_sum_test)

# Verify that bool_rejection_test fails to compile (compile-time bool rejection test)
# This is a negative test - we expect it to FAIL to compile
if(NOT BOOL_REJ_TEST_COMPILE_SUCCEEDED)
    message(STATUS "Bool rejection test PASSED: sum(bool, ...) correctly rejected at compile time")
else()
    message(FATAL_ERROR "Bool rejection test FAILED: sum(bool, ...) should not compile!")
endif()

set(BOOL_REJ_TEST_COMPILE_DIR "${CMAKE_BINARY_DIR}/bool_rejection_test_compile")
file(MAKE_DIRECTORY "${BOOL_REJ_TEST_COMPILE_DIR}")

set(BOOL_REJ_TEST_COMPILE_CMAKE "${BOOL_REJ_TEST_COMPILE_DIR}/CMakeLists.txt")
file(WRITE "${BOOL_REJ_TEST_COMPILE_CMAKE}" "")

file(COPY "${CMAKE_SOURCE_DIR}/test_bool_rejection.cpp" "${BOOL_REJ_TEST_COMPILE_DIR}/"
    DESTINATION "${BOOL_REJ_TEST_COMPILE_DIR}/")

try_compile(BOOL_REJ_TEST_COMPILE_SUCCEEDED
    ${CMAKE_BINARY_DIR}
    "${BOOL_REJ_TEST_COMPILE_DIR}/test_bool_rejection.cpp"
    COMPILE_DEFINITIONS "${CMAKE_SOURCE_DIR}"
)

if(NOT BOOL_REJ_TEST_COMPILE_SUCCEEDED)
    message(STATUS "Bool rejection test PASSED: sum(bool, ...) correctly rejected at compile time")
else()
    message(FATAL_ERROR "Bool rejection test FAILED: sum(bool, ...) should not compile!")
endif()

add_executable(bool_rejection_check bool_rejection_check.cpp)
add_test(NAME bool_rejection_test COMMAND bool_rejection_check)
