# Add your test targets here
add_executable(test_mylib test.cpp)
target_link_libraries(test_mylib gtest_main gtest seq)