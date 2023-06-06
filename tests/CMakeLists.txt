add_executable(algos_tests algos_tests.cpp)

target_link_libraries(algos_tests ${LIB_TARGET} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(algos_tests)

set_target_properties(
        algos_tests
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)