# лабораторная работа №2
## Работу выполнил: Ивашков Станислав | ПМ-31
## Задание - 1
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ cc ./src/swap/main.c ./src/swap/swap.c  -o ./src/swap/swap
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ ./src/swap/swap
b a
```
## Задание - 2
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ cc ./src/revert_string/main.c ./src/revert_string/revert_string.c -o ./src/revert_string/revert_string
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ ./src/revert_string/revert_string test_string
Reverted: gnirts_tset
```
## Задание - 3
Static link:
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ mkdir src/revert_string/static
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ gcc -c src/revert_string/revert_string.c  -o src/revert_string/static/revert_string.o
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ ar rcs src/revert_string/static/libtq84.a -o src/revert_string/static/revert_string.o
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ gcc src/revert_string/main.o -Lsrc/revert_string/static -ltq84 -o src/revert_string/static/statically-linked
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ ./src/revert_string/static/statically-linked test_string
Reverted: gnirts_tset
```
Link dynamically
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ mkdir src/revert_string/shared
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ gcc -c -fPIC src/revert_string/revert_string.c -o src/revert_string/shared/revert_string.o
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ gcc -shared src/revert_string/shared/revert_string.o -o src/revert_string/shared/librevert_string.so
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ gcc src/revert_string/main.o -Lsrc/revert_string/shared -lrevert_string -o src/revert_string/shared/dynamically-linked
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ LD_LIBRARY_PATH=$(pwd)/src/revert_string/shared
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ export LD_LIBRARY_PATH
@drkrops ➜ /workspaces/os_lab_2019/lab2 (lab2) $ ./src/revert_string/shared/dynamically-linked test_String
Reverted: gnirtS_tset
```
## Задание - 4
Сказали что не надо:(