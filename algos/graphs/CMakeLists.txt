add_executable(turtle turtle.cpp)
add_executable(turtle_1_k turtle_1_k.cpp)
add_executable(is_looped is_looped.cpp)
add_executable(top_sort top_sort.cpp)
add_executable(drones drones.cpp)
add_executable(scc scc.cpp)
add_executable(bridges bridges.cpp)
add_executable(points points.cpp)
add_executable(coruscant coruscant.cpp)
add_executable(double_attack double_attack.cpp)

set_target_properties(
        turtle
        turtle_1_k
        is_looped
        top_sort
        drones
        scc
        bridges
        points
        coruscant
        double_attack
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)