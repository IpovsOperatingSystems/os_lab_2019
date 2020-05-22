cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" -B./build -H./
cmake --build ./build -- -j4
./build/zombies