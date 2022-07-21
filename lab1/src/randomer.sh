#!/bin/bash
A=0
while [ $A -lt 150 ]
do 
od -A n -t d -N 1 /dev/urandom >> numbers.txt
let "A=A+1"
done