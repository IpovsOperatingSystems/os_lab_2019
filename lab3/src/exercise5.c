#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

    pid_t child_pid = fork();

    if (child_pid == 0) {
        execl("./sequential_min_max", "./sequential_min_max", argv[1], argv[2], NULL);
    }
    return 0;
}