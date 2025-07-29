# Add the executable
add_executable(element_at element_at.cpp)

# Specify the C++ standard
set_target_properties(element_at PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Link the element_at executable to the element_at library
target_link_libraries(element_at PRIVATE element_at_lib)