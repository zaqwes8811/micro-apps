cmake_minimum_required(VERSION 3.10)
project(toMicroCpp VERSION 1.0)

# Find required packages
find_package(Threads REQUIRED)

# Add all C++ files as separate executables
add_executable(VectorTest VectorTest.cpp Vector.hpp)
target_link_libraries(VectorTest PRIVATE Threads::Threads)

add_executable(Vec2Test Vec2Test.cpp Vec2.hpp)
target_link_libraries(Vec2Test PRIVATE Threads::Threads)
