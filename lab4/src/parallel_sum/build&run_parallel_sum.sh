#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" -B./build -H./
cmake --build ./build -- -j4
./build/parallel_sum --threads_num 1 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 2 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 3 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 4 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 5 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 6 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 7 --seed 34512 --array_size 60000000
./build/parallel_sum --threads_num 8 --seed 34512 --array_size 60000000