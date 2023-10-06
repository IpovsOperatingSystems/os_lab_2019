# Лабораторная работа №1
## Работу выполнил: Ивашков Станислав | ПМ-31

## Задание - 1
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ ./src/background.sh &
[1] 15224
1 sec
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ 2 sec
3 sec
4 sec
5 sec
6 sec
7 sec
8 sec
9 sec
10 sec
Done!

[1]+  Done                    ./src/background.sh

@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ touch src/test.txt
second row@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ cat src/test.txt 
test message
second row
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ clear
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ wc src/test.txt 
 2  4 24 src/test.txt
```
## Задание - 2
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ cat src/cake_rhymes.txt | grep 'cake' > src/with_cake.txt
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ touch src/file.txt
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ rm src/file.txt > /dev/null
```
## Задание - 3
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ chmod u+x src/hello.sh 
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ ./src/hello.sh 
Hello, world!

@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ touch src/ex3
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ chmod u+x src/ex3 
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ ./src/ex3 
2023-10-06 14:20:28
/usr/local/rvm/gems/ruby-3.2.2/bin:/usr/local/rvm/gems/ruby-3.2.2@global/bin:/usr/local/rvm/rubies/ruby-3.2.2/bin:/vscode/bin/linux-x64/e7e037083ff4455cf320e344325dacb480062c3c/bin/remote-cli:/home/codespace/.local/bin:/home/codespace/.dotnet:/home/codespace/nvm/current/bin:/home/codespace/.php/current/bin:/home/codespace/.python/current/bin:/home/codespace/java/current/bin:/home/codespace/.ruby/current/bin:/home/codespace/.local/bin:/usr/local/python/current/bin:/usr/local/py-utils/bin:/usr/local/oryx:/usr/local/go/bin:/go/bin:/usr/local/sdkman/bin:/usr/local/sdkman/candidates/java/current/bin:/usr/local/sdkman/candidates/gradle/current/bin:/usr/local/sdkman/candidates/maven/current/bin:/usr/local/sdkman/candidates/ant/current/bin:/usr/local/rvm/gems/default/bin:/usr/local/rvm/gems/default@global/bin:/usr/local/rvm/rubies/default/bin:/usr/local/share/rbenv/bin:/usr/local/php/current/bin:/opt/conda/bin:/usr/local/nvs:/usr/local/share/nvm/versions/node/v20.8.0/bin:/usr/local/hugo/bin:/usr/local/dotnet/current:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/rvm/bin
```
## Задание - 4
```bash
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ touch src/average.sh
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ sh src/average.sh 19 1 2 6
Число аргументов: 4
Среднее арифметическое: 7

@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ ./src/numbers 
@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ chmod u+x ./src/average.sh 

@drkrops ➜ /workspaces/os_lab_2019/lab1 (lab1) $ ./src/average.sh $(cat ./src/number.txt)
Число аргументов: 150
Среднее арифметическое: -10
```