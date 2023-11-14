#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid;

    // Создаем дочерний процесс
    if ((child_pid = fork()) == 0) {
        // Это код дочернего процесса
        printf("Child process with PID %d is exiting\n", getpid());
        exit(0); // Дочерний процесс завершает выполнение
    } else if (child_pid > 0) {
        // Это код родительского процесса
        printf("Parent process is sleeping\n");

        // Родительский процесс спит 10 секунд
        sleep(10);

        // Родительский процесс завершает выполнение
        printf("Parent process is exiting\n");
    } else {
        // Обработка ошибки при создании дочернего процесса
        perror("fork");
        return 1;
    }

    return 0;
}
