#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0) {
        // Этот код выполняется в дочернем процессе

        // Используйте функцию exec() для запуска sequential_min_max
        if (execl("sequential_min_max", "sequential_min_max", "42", "100", NULL) == -1) {
            perror("execl");
            exit(1);
        }
    } else {
        // Этот код выполняется в родительском процессе

        // Дожидаемся пока завершится дочерний процесс
        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}