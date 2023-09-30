add_executable(bridges_tests bridges.cpp)
add_executable(binary_heap_test binary_heap_test.cpp)
add_executable(sort_test sort_test.cpp)

target_link_libraries(bridges_tests GTest::gtest_main)
target_link_libraries(binary_heap_test GTest::gtest_main)
target_link_libraries(sort_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(bridges_tests)
gtest_discover_tests(binary_heap_test)
gtest_discover_tests(sort_test)

set_target_properties(
        bridges_tests
        binary_heap_test
        sort_test
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)