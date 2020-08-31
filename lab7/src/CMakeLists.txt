cmake_minimum_required(VERSION 3.1)
project(Parallel_Factorial LANGUAGES C)

add_subdirectory(./num_partition)
add_executable(Parallel_Factorial main.c)
target_link_libraries(Parallel_Factorial NumberPartition)

find_package(Threads REQUIRED)
target_link_libraries(Parallel_Factorial ${CMAKE_THREAD_LIBS_INIT})