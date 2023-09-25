#!bin/bash

for ((i=1;i<152;i++))
do
		od -vAn -N1 -td1 < /dev/random >> numbers.txt
done
sed s/' '//g -i numbers.txt

