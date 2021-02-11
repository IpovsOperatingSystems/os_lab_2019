#! /bin/sh 

count=1
sum=0
while ((-n "$1"))
do
sum = $(( $sum + $1 ))
count = $(( $count + 1 ))
shift
done
res = $(($sum / $count))
echo "Среднее арифметическое: $res"