# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.12)

# Create an interface library for seq.h
add_library(seq INTERFACE)

# Specify the include directories for the library
target_include_directories(seq INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add seq.h as a source in the library
target_sources(seq INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/seq.h)
# Add any other dependencies or compile options as needed

# Optionally, you can set the library properties
# set_target_properties(seq PROPERTIES ...)

# Optionally, you can install the library
# install(TARGETS seq DESTINATION lib)
# install(FILES seq.h DESTINATION include)

# Create an interface library for element_at.h
add_library(element_at_lib INTERFACE)

# Specify the include directories for the library
target_include_directories(element_at_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add element_at.h as a source in the library
target_sources(element_at_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/element_at.h)