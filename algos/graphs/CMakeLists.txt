add_executable(turtle turtle.cpp)
add_executable(turtle_1_k turtle_1_k.cpp)
add_executable(is_looped is_looped.cpp)
add_executable(top_sort top_sort.cpp)

set_target_properties(
        turtle
        turtle_1_k
        is_looped
        top_sort
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)