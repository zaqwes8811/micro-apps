cmake_minimum_required(VERSION 3.10)
project(VectorProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VectorTest src/VectorTest.cpp src/Vec2Test.cpp)
include_directories(src)
