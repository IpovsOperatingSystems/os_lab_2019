cmake_minimum_required(VERSION 3.1)
project(OSLAB5 LANGUAGES C)

add_executable(factorial factorial.c)

find_package(Threads REQUIRED)

target_link_libraries(factorial ${CMAKE_THREAD_LIBS_INIT})