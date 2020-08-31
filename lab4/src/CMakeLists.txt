cmake_minimum_required(VERSION 3.1)
project(OSLAB4 LANGUAGES C)

add_executable(parallel_min_max parallel_min_max.c utils.c find_min_max.c)

target_include_directories(parallel_min_max PUBLIC ./)

set_target_properties(parallel_min_max PROPERTIES LINKER_LANGUAGE C)