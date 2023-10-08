# Лабораторная работа №3
## Выполнил: Ивашков Станислав | ПМ-31
## Задание - 1
Что делает программа в sequiential_min_max.c. Она принимает на вход `seed` и `array_size`, и по ним генерирует массив.
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab3 (lab3) $ gcc src/sequential_min_max.c src/utils.c  src/find_min_max.c  -o src/sequential_min_max
@drkrops ➜ /workspaces/os_lab_2019/lab3 (lab3) $ ./src/sequential_min_max 2 10
min: 142559277
max: 1738766719
```
## Задание - 2-3
* fork() создает новый процесс, дублируя вызывающий процесс. Новый процесс называется дочерним процессом. Вызывающий процесс называется родительским процессом.
* Pipe() создает канал — однонаправленный канал данных, который можно использовать для межпроцессного взаимодействия. Массив Pipefd используется для возврата двух файловых дескрипторов, относящихся к концам канала. Pipefd[0] относится к концу канала чтения. Pipefd[1] относится к концу канала записи.
```bash
@drkrops ➜ /workspaces/os_lab_2019 (master) $ gcc lab3/src/parallel_min_max.c lab3/src/find_min_max.c lab3/src/utils.c  -o lab3/src/parallel_min_max
@drkrops ➜ /workspaces/os_lab_2019 (master) $ ./lab3/src/parallel_min_max --seed "123" --array_size "50" --pnum "5"
Min: 7523372
Max: 2097911480
Elapsed time: 0.650000ms
@drkrops ➜ /workspaces/os_lab_2019 (master) $ ./lab3/src/parallel_min_max --seed "123" --array_size "50" --pnum "5" --by_files
Min: 7523372
Max: 2097911480
Elapsed time: 1.389000ms
```
## Задание - 4-5
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab3/src (master) $ make
gcc -o utils.o -c utils.c -I.
gcc -o find_min_max.o -c find_min_max.c -I.
gcc -o sequential_min_max find_min_max.o utils.o sequential_min_max.c -I.
gcc -o parallel_min_max utils.o find_min_max.o parallel_min_max.c -I.
gcc -o make_exercise_5 exercise5.c -I.
chmod u+x make_exercise_5
@drkrops ➜ /workspaces/os_lab_2019/lab3/src (master) $ ./sequential_min_max 132 20
min: 8669557
max: 2001248850
@drkrops ➜ /workspaces/os_lab_2019/lab3/src (master) $ ./parallel_min_max --seed "132" --array_size "20" --pnum "4"
Min: 8669557
Max: 2001248850
Elapsed time: 0.500000ms
@drkrops ➜ /workspaces/os_lab_2019/lab3/src (master) $ ./make_exercise_5 132 20
min: 8669557
max: 2001248850
```
