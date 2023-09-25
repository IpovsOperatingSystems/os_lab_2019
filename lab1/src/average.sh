#!bin/bash
count=0
sum=0
while read arg
do
	count=$(($count+1))
	sum=$(($sum+$arg))
done
dev=$(($sum / $count))
echo $dev
echo $count
