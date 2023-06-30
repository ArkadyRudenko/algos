add_executable(mashup mashup.cpp)
add_executable(k_stat k_stat.cpp)
add_executable(str test.cpp)

set_target_properties(
        mashup
        k_stat
        str
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
