cmake_minimum_required(VERSION 3.28)
project(slk_spike)

set(CMAKE_CXX_STANDARD 14)

find_library(TSK_LIB tsk REQUIRED)

add_executable(slk_spike
        main.cpp)

target_link_libraries(slk_spike ${TSK_LIB})
