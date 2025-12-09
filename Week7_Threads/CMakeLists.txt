cmake_minimum_required(VERSION 3.0)
project(Week7_Threads)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(Week7_Threads Week7_Threads.cpp)
target_link_libraries(Week7_Threads ${CMAKE_THREAD_LIBS_INIT})