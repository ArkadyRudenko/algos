add_executable(dijkstra dijkstra.cpp)
add_executable(maps maps.cpp)

set_target_properties(
        dijkstra
        maps
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
