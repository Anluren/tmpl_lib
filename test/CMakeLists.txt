# Enable testing
enable_testing()

# Add your test targets here
add_executable(test_mylib test.cpp)
target_link_libraries(test_mylib gtest_main gtest seq)

# Include GoogleTest module
include(GoogleTest)

# Discover unit tests
gtest_discover_tests(test_mylib)