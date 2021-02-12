#!/bin/bash

total=0
for var in $@; do

total=$(($total+var))

done

echo $#
echo $(($total/$#))