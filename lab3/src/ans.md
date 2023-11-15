# Отчет по ЛР №3

## Задание 1

### input: 
```shell
cc sequential_min_max.c find_min_max.c utils.c -o task1 && ./task1 2 6 
```

### output:
```shell
min: 0
max: 5
```

## Задание 2-3

### input with flag --by_files:
```shell
cc parallel_min_max.c find_min_max.c utils.c -o task2 && ./task2 --seed 2 --array_size 4 --pnum 2 --by_files
```

### output:
```shell
Min: 0
Max: 3
Elapsed time: 0.238000ms
```

### input without flag:

```shell
cc parallel_min_max.c find_min_max.c utils.c -o task2 && ./task2 --seed 2 --array_size 4 --pnum 2           
```
### output:
```shell
Min: 0
Max: 3
Elapsed time: 0.165000ms
```

## Задание 4

Вот краткое описание того, что делает этот Makefile:

CC=gcc устанавливает компилятор на gcc. <br>
<br>
CFLAGS=-I. устанавливает флаги для компилятора, в данном случае, это включает текущий каталог для поиска заголовочных файлов во время компиляции. <br>
<br>
Цель all указывает, что при выполнении make all будут построены sequential_min_max, parallel_min_max, find_min_max.o и utils.o. <br>
<br>
Определены правила для построения исполняемых файлов sequential_min_max и parallel_min_max, а также объектных файлов utils.o и find_min_max.o. <br>
<br>
Цель clean определена для удаления скомпилированных объектных файлов и исполняемых файлов при выполнении make clean. <br>


## Задание 5

exec позволяет запускать внутри своей программы, другие скрипты или программы, это получается сделать из-за того, что текущ. процесс выполняет новый код