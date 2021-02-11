#!/bin/sh 

sum=0

for i in $@
do
sum=$(($sum+$i))
done

echo "Среднее значение = $(($sum/$#))"
echo "Число элементов = $#"

# cat numbers.txt | xargs ./average.sh