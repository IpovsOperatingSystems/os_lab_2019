cmake_minimum_required(VERSION 3.1)
project(parallel_sum LANGUAGES C)

add_executable(parallel_sum parallel_sum.c sum.c)

find_package(Threads REQUIRED)

target_link_libraries(parallel_sum ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(parallel_sum PUBLIC ./)

add_library(utils ../utils.c)

target_include_directories(utils PUBLIC ../)

target_link_libraries(parallel_sum utils)

set_target_properties(parallel_sum PROPERTIES LINKER_LANGUAGE C)