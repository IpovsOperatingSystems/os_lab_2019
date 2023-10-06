# Лабораторная работа №0
## Выполнил: Ивашков Станислав | ПМ-31

##  Задание - 1
Сделано
##  Задание - 2
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ mkdir hello
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ touch hello/empty
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ cp src/hello.c hello/
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ mv hello/hello.c hello/newhello.c
```
##  Задание - 3
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ ../update.sh 
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ ping ya.ru -c 5

--- ya.ru ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 4004ms
```
##  Задание - 4
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ gcc hello/newhello.c -o hello/hello_world
@drkrops ➜ /workspaces/os_lab_2019/lab0 (lab0) $ ./hello/hello_world 
Hello World!
```