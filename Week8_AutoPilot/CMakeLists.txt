cmake_minimum_required(VERSION 3.0)

project(Week8_AutoPilot_Visual)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(Week8_AutoPilot_Visual Week8_AutoPilot_Visual.cpp)
target_link_libraries(Week8_AutoPilot_Visual Threads::Threads)