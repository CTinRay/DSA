#!/bin/bash


while read line           
do           
    echo $line | $1
done < /dev/stdin