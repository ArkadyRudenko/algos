add_executable(bin_search bin_search.cpp)
add_executable(min_limos min_limos.cpp)
add_executable(loop_offset loop_offset.cpp)
add_executable(min_stack min_stack.cpp)
add_executable(hippogriff hippogriff.cpp)
add_executable(hat hat.cpp)
add_executable(charmbaton charmbaton.cpp)
add_executable(av_noise av_noise.cpp)
add_executable(nimbus nimbus.cpp)

set_target_properties(
        min_limos
        bin_search
        av_noise
        charmbaton
        hat
        hippogriff
        min_stack
        loop_offset
        nimbus
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
