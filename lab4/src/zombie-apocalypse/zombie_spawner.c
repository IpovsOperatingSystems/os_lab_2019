#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int main() {
    pid_t child_pid;
    printf("Started zombie apocalypse!");
    child_pid = fork();
    if (child_pid > 0) {
        while (true);
    } else {
        exit(0);
    }

    return 0;
}


