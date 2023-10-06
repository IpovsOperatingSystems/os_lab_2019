#! /bin/sh
len=$#
sum=0
for arg in "$@"
do
  sum=$(($sum+${arg}))
done
avg=$(($sum / $len))
expr "Число аргументов: $len"
expr "Среднее арифметическое: $avg"