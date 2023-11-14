# Лабораторная работа №3
## Выполнил: Ивашков Станислав | ПМ-31
## Задание - 1
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab4/src (lab4) $ gcc ../../lab3/src/utils.c ../../lab3/src/find_min_max.c parallel_min_max.c -o parallel_min_max
@drkrops ➜ /workspaces/os_lab_2019/lab4/src (lab4) $ ./parallel_min_max --seed "123" --array_size "50" --pnum "2"
Min: 7523372
Max: 2097911480
Elapsed time: 0.195000ms
@drkrops ➜ /workspaces/os_lab_2019/lab4/src (lab4) $ ./parallel_min_max --seed "123" --array_size "50" --pnum "5" --timeout "10"
Min: 7523372
Max: 2097911480
Elapsed time: 0.565000ms
```

## Задание - 2
Зомби-процесс – это процесс, который завершил выполнение своего кода, но ещё не был полностью удален из системы. Информация о завершенном процессе сохраняется до тех пор, пока родительский процесс не запросит эту информацию с помощью системного вызова wait или waitpid. После получения статуса завершения дочернего процесса родительский процесс может вызвать waitpid с опцией WNOHANG, чтобы полностью удалить зомби-процесс из системы.

Как появляются зомби-процессы:
* Процесс завершается:
* Сохранение статуса завершения:
    Когда процесс завершается, ядро сохраняет его статус завершения в таблице процессов.
* Ожидание родителя:
    Зомби-процесс ожидает, пока его родительский процесс вызовет системный вызов wait или waitpid для получения статуса завершения.
Как исчезают зомби-процессы:
    Родительский процесс вызывает wait или waitpid

```bash
@drkrops ➜ /workspaces/os_lab_2019/lab4/src (lab4) $ touch zombie_demo.c
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ gcc ./zombie_demo.c -o zombie_demo
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ ./zombie_demo 
Parent process is sleeping
Child process with PID 6099 is exiting
Parent process is exiting
```

## Задание - 3
Виртуальная память - это концепция, которая позволяет программам использовать больше памяти, чем фактически доступно в физической оперативной памяти. Каждая программа работает в своем собственном виртуальном адресном пространстве, которое может быть гораздо больше, чем физическая память на компьютере. Виртуальная память позволяет программам иметь искусственное впечатление большего объема памяти, чем действительно есть.\
Когда программа обращается к виртуальной памяти, операционная система переводит эти адреса в физические адреса в реальной памяти. Она может загружать и выгружать разделы программы на диск (или другие хранилища), чтобы освободить место в физической памяти для других процессов.
```bash
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ gcc process_memory.c -o process_memory
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ ./process_memory

Address etext: 0x55832fed9509 
Address edata: 0x55832fedc018 
Address end  : 0x55832fedc050 
ID main 	 is at virtual address: 0x55832fed9249
ID showit 	 is at virtual address: 0x55832fed9407
ID cptr 	 is at virtual address: 0x55832fedc010
ID buffer1 	 is at virtual address: 0x55832fedc030
ID i 	 is at virtual address: 0x7ffddfa21b94
A demonstration
ID buffer2 	 is at virtual address: 0x7ffddfa21b70
Alocated memory at 0x55833085e6b0
This message is output by the function showit()
```

- **etext**: Место, где хранится код программы, который выполняется.
- **edata**: Область для данных, в которой хранятся значения переменных, с которыми программа начинает свою работу.
- **end**: Здесь заканчивается программа, и здесь хранятся глобальные переменные, которые не имеют начальных значений.

## Задание - 4
```bash
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ make
gcc -o utils.o -c utils.c -I.
gcc -o find_min_max.o -c find_min_max.c -I.
gcc -o sequential_min_max.o find_min_max.o utils.o sequential_min_max.c -I.
gcc -o parallel_min_max.o utils.o find_min_max.o parallel_min_max.c -I.
```

## Задание - 5
### Внесенные изменения в код:

1. **Обработка аргументов командной строки:**
2. **Обработка ошибок для аргументов командной строки:**
3. **Генерация массива:**
   - Использована функция `GenerateArray` для создания массива со случайными значениями на основе заданного seed.
4. **Настройка аргументов для потоков:**
   - Изменен цикл для настройки аргументов потоков (`args`). Теперь каждому потоку назначен определенный диапазон массива для обработки.
5. **Измерение времени:**
6. **Вывод затраченного времени:**
   - Выведено затраченное время вместе с общей суммой.

```bash
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ gcc -o parallel_sum  parallel_sum.c utils.c -pthread
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ ./parallel_sum
Total: 0
Elapsed time: 0.000000ms
stas@stas-VirtualBox:~/Desktop/study/os/os_lab_2019/lab4/src$ ./parallel_sum --seed "123" --array_size "50" --pnu "5"
Total: 696866619
Elapsed time: 0.314000ms
```

## Задание - 6
Создал :)
