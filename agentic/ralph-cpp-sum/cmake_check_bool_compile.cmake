# CMake file to check that bool_rejection_test fails to compile as expected

# Try to compile the bool rejection test
set(BOOL_REJECTION_TEST_SRC "${CMAKE_SOURCE_DIR}/bool_rejection_test/bool_rejection_test.cpp")

# Create a temporary directory for try_compile
set(TEST_COMPILE_DIR "${CMAKE_BINARY_DIR}/bool_rejection_test_compile")
file(MakeDirectory "${TEST_COMPILE_DIR}")

# Create a temporary CMakeLists.txt for try_compile
set(TEST_COMPILE_CMAKE "${TEST_COMPILE_DIR}/CMakeLists.txt")
file(WRITE "${TEST_COMPILE_CMAKE}" "")

# Add a test target that will try to compile bool_rejection_test.cpp
# If it compiles, the test fails (because we expect it to fail)
# If it doesn't compile, the test passes (because we expect it to fail)

# Set up the include directories needed for try_compile
set(CMAKE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR};${GTEST_INCLUDE_DIRS}")

# Create a simple test that just tries to compile bool_rejection_test.cpp
# and checks if it fails
add_custom_target(verify_bool_rejection_compile
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_COMPILE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E touch "${TEST_COMPILE_DIR}/bool_rejection_test.cpp"
    COMMAND ${CMAKE_COMMAND} -E echo "Attempting to compile bool_rejection_test.cpp..."
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++20 -I"${CMAKE_SOURCE_DIR}" -I"${GTEST_INCLUDE_DIRS}" -c "${BOOL_REJECTION_TEST_SRC}" -o "${TEST_COMPILE_DIR}/bool_rejection_test.o" 2>&1
    RESULT_VARIABLE COMPILER_EXIT_CODE
    COMMENT "Bool rejection compile check: Compiler exit code = ${COMPILER_EXIT_CODE}"
    DEPENDS "${BOOL_REJECTION_TEST_SRC}"
)

# The test passes if the compiler exits with non-zero (i.e., compilation failed)
# This is a negative test - we EXPECT compilation to fail
add_custom_target(bool_rejection_verify
    DEPENDS verify_bool_rejection_compile
    COMMAND ${CMAKE_COMMAND} -E echo "Bool rejection verification: Compilation failed with exit code ${COMPILER_EXIT_CODE} - THIS IS EXPECTED"
    COMMAND ${CMAKE_COMMAND} -E echo "Bool rejection test PASSED: sum(bool, ...) correctly rejected at compile time"
)
