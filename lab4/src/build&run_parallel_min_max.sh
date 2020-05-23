#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" -B./build -H./
cmake --build ./build -- -j4
./build/parallel_min_max --seed 34434 --array_size 34454545 --pnum 4 --timeout 2 -f