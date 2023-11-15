#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
    pid_t child_pid = fork();
    if (child_pid < 0) {
        printf("Error open fork.");
        return 1;
    } else if (child_pid == 0) {
        execv("sequential_min_max", argv);
        return 0;
    }
    wait(0);
    return 0;
}