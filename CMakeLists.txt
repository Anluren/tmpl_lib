cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Add GoogleTest's CMakeLists.txt to the build
add_subdirectory(googletest)

add_subdirectory(include)

# Link GoogleTest to the test executable
add_subdirectory(test)
