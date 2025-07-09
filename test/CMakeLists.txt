# Enable testing
enable_testing()

# Add your test targets here
add_executable(test_mylib test.cpp)
target_link_libraries(test_mylib gtest_main gtest seq)

# Add test for check_env
add_executable(test_check_env test_check_env.cpp)
target_include_directories(test_check_env PRIVATE ${CMAKE_SOURCE_DIR}/util_macro)
target_link_libraries(test_check_env gtest_main gtest)

# Include GoogleTest module
include(GoogleTest)

# Discover unit tests
gtest_discover_tests(test_mylib)
gtest_discover_tests(test_check_env)