add_executable(tests tests.cpp)
add_executable(bridges_tests bridges.cpp)

target_link_libraries(tests ${LIB_TARGET} GTest::gtest_main)
target_link_libraries(bridges_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
gtest_discover_tests(bridges_tests)

set_target_properties(
        tests
        bridges_tests
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)