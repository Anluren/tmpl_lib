cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY git@github.com:google/googletest.git
    GIT_TAG release-1.11.0
)

# Download and unpack googletest at configure time
FetchContent_MakeAvailable(googletest)

# Add your source files here
add_executable(MyProject main.cpp)

# Link GoogleTest to your project
target_link_libraries(MyProject gtest gtest_main)

# Ensure that the tests are built
enable_testing()
add_test(NAME MyProjectTest COMMAND MyProject)