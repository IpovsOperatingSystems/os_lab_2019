#!
gcc -c -fPIC main.c -o main.o
gcc -c -fPIC revert_string.c -o revert_string.o
gcc -shared revert_string.o -o librevert_string.so
gcc main.o -L./ -lrevert_string -o reverted
LD_LIBRARY_PATH=$(pwd) ./reverted 9 123456789