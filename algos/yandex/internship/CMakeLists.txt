add_executable(job_task1 task1.cpp)
add_executable(job_task2 task2.cpp)
add_executable(job_task3 task3.cpp)
add_executable(job_task5 task5.cpp)
add_executable(leetcode leetcode.cpp)
add_executable(tink1 tink1.cpp)
add_executable(tink5 tink5.cpp)
add_executable(tink6 tink6.cpp)

set_target_properties(
        job_task1
        job_task2
        job_task3
        job_task5
        leetcode
        tink1
        tink5
        tink6
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
