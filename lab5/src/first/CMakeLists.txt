cmake_minimum_required(VERSION 3.1)
project(OSLAB5 LANGUAGES C)

add_executable(mutex mutex.c)
add_executable(nomutex nomutex.c)

find_package(Threads REQUIRED)

target_link_libraries(mutex ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(nomutex ${CMAKE_THREAD_LIBS_INIT})