#!bin/sh

touch numbers.txt

for ((i=0;i<150;i++))
do
"$(sudo od -An -N1 -i /dev/random)>numbers.txt"
done
