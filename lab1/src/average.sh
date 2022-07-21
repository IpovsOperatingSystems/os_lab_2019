#!/bin/bash
aver=0
count=0
while [  -n "$1" ]
do 
((aver=aver+$1))
((count=count + 1))
shift
done
let "aver = aver / count"
echo "averege" $aver
echo "count" $count
