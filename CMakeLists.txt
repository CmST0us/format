cmake_minimum_required(VERSION 3.0)

project(format_test)

# Set Compiler Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

# Target
add_executable(
	test_format
    test_format.cpp
)

