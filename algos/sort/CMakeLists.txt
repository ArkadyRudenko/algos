add_executable(mashup mashup.cpp)
add_executable(k_stat k_stat.cpp)
add_executable(str test.cpp)
add_executable(binary_heap binary_heap.cpp)

set_target_properties(
        mashup
        k_stat
        str
        binary_heap
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
