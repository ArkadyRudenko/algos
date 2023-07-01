cmake_minimum_required(VERSION 3.25)
project(algos)

option(TESTS "Build algos tests" ON)

# ---------- Google Tests ----------
set(CMAKE_CXX_STANDARD 14)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
# --------------

set(LIB_TARGET algos)

get_filename_component(LIB_PATH "algos" ABSOLUTE)
get_filename_component(LIB_INCLUDE_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

message("Enable ADDRESS_SANITIZE")
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# ADD algos Lib
add_library(${LIB_TARGET} STATIC ${LIB_HEADERS})
target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})

set_target_properties(
        ${LIB_TARGET}
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

add_subdirectory(algos)

if(TESTS)
    message("TESTS ENABLED")
    add_subdirectory(tests)
endif()