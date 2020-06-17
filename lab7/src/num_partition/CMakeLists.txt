cmake_minimum_required(VERSION 3.1)
project(Parallel_Factorial LANGUAGES C)

add_library(NumberPartition number_partition.c)
target_include_directories(NumberPartition PUBLIC ./)