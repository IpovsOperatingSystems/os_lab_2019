#!/bin/sh
n=0
res=0
while read input
do
	for m in $input
	do
        
		res=$(($res+$m))
		n=$(($n+1))
	done
done
res=$(($res/150))
echo "Кол-во чисел: $n\nСреднее арифметическое: $res"
exit 0