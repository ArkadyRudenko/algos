add_subdirectory(internship)

add_executable(task1 task1.cpp)
add_executable(leet_code leet_code.cpp)

set_target_properties(
        task1
        leet_code
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
