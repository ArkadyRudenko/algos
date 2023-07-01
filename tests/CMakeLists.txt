add_executable(tests tests.cpp)

target_link_libraries(tests ${LIB_TARGET} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

set_target_properties(
        tests
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)