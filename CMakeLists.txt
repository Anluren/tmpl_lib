cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add GoogleTest's CMakeLists.txt to the build
add_subdirectory(googletest)

add_subdirectory(include)

# Link GoogleTest to the test executable
add_subdirectory(test)

# add_subdirectory(ut)