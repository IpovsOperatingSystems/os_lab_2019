#!/bin/bash

echo 'Количество входных аргументов: ' $#

i=0
sum=0
for arg in "$@" 
do
	sum=$(($sum + ${arg}))
	i=$(($i+1))
done
echo 'Среднее арифметическое входных аргументов: '
expr $sum / $i

exit 0
