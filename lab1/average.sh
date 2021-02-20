#!/bin/bash

sum=0
count=0
for i in $(cat $1)
do 
sum=$((sum+$i))
count=$((count+1))
done
echo $(($sum/$count))
echo $count
