cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GoogleTest
add_subdirectory(dependencies/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add your source files here
add_executable(MyProject main.cpp)

# Link GoogleTest to your project
target_link_libraries(MyProject gtest gtest_main)
