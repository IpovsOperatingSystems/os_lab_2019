#!
#Start this script after make_shared
gcc -c tests.c -L./ -o tests.o
gcc tests.o -L./ -lrevert_string -lcunit -o tests
LD_LIBRARY_PATH=$(pwd) ./tests