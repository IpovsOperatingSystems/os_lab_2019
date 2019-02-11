# Лабораторная работа №1

## Задание 1

### Необходимые знания

* `&` в `shell`
* Как работает команда `cat`
* Как работает команда `clear`
* Как работает команда `wc`

### Задание

* Запустите скрипт `background.sh` в фоновом режиме.
* Создайте текстовый файл и выведите его содержимое на экран терминала.
* Очистите окно терминала.
* Посчитайте количество символов в файле.


### Ресурсы

* [Небольшой справочник по основным утилитам командной строки, которые можно найти почти в любом дистрибутиве Linux.](https://searchdatacenter.techtarget.com/tutorial/77-Linux-commands-and-utilities-youll-actually-use)
* [Про `&` в `shell`.](https://stackoverflow.com/questions/13338870/what-does-at-the-end-of-a-linux-command-mean)

# Задание 2

### Необходимые знания

* Как работает команда `grep`
* Как работает редирект в `bash`
* Что такое `pipe` в `bash`
* Специальные девайсы в *nix системах (`/dev/null`, `/dev/full` и т.д.)

### Задание

* С помощью команды `grep`, используя `pipe` и редирект (`>`), запишите в файл `with_cake.txt` все строчки из файла `cake_rhymes.txt`, в которых есть слово `cake`.

* Сделайте команду `rm` "тихой", используя редирект в `/dev/null`.

### Ресурсы

* [Небольшой справочник по основным утилитам командной строки, которые можно найти почти в любом дистрибутиве Linux.](https://searchdatacenter.techtarget.com/tutorial/77-Linux-commands-and-utilities-youll-actually-use)

* [Про `pipe`](https://www.geeksforgeeks.org/piping-in-unix-or-linux/)

* [Про то, как работают редиректы.](http://wiki.bash-hackers.org/howto/redirection_tutorial)

* [Про `/dev/null`, `/dev/full` и другие "special device files" в *nix системах](https://en.wikipedia.org/wiki/Device_file#Pseudo-devices)

# Задание 3

### Необходимые знания

* Права в linux
* Команда `chmod`
* Переменные окружения
* Команда `date` в bash
* Что такое `shebang`

### Задание

* Сделайте файл `hello.sh` исполняемым, выполните его.

* Напишите `bash` скрипт, который выводит текущий путь, текущую дату и время, а также содержимое переменной окружения `PATH`.

### Ресурсы

* [Вводная статья про то, как вообще работать с правами в Linux](https://www.linux.com/learn/understanding-linux-file-permissions)

* [Как напечатать дату и время.](https://unix.stackexchange.com/questions/85982/date-time-in-linux-bash)

* [Описание того, как работают переменные окружения.](https://www.digitalocean.com/community/tutorials/how-to-read-and-set-environmental-and-shell-variables-on-a-linux-vps)

# Задание 4\* (повышенной сложности)

### Необходимые знания

* Как работать с аргументами в `bash` скриптах
* Как работает команда `od`
* Специальные устройства в *nix системах
* Как работает редирект

### Задание

* Напишите скрипт `average.sh`, который выводит количество и среднее арифмитическое его входных аргументов.

* С помощью `bash` и `dev/random` создайте файл `numbers.txt` из 150 рандомных чисел.

* "Скормите" скрипту `average.sh` значения из файла `numbers.txt`.

### Ресурсы

* [Google](https://www.google.com)
