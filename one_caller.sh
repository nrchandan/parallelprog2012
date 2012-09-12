#!/bin/bash

if [[ "$1" = "" ]] ; then
	echo "Argument missing"
	exit
fi

perf stat -r 5 -e cycles -e instructions -e cache-references -e cache-misses -e L1-dcache-loads -e L1-dcache-loads-misses -o temp_$1.txt ./ipTranspose -basic -n $1 -noio > /dev/null
tail -11 temp_$1.txt > one_caller_temp1.txt

#echo "Command run was : " $(head -1 one_caller_temp1.txt)
#echo "Cycles are : " $(cat one_caller_temp1.txt | grep -o '[0-9,]* cycles' | grep -o '[0-9,]*') 
#echo "Instructions are : " $(cat one_caller_temp1.txt | grep -o '[0-9,]* instructions' | grep -o '[0-9,]*') 
#echo "Ins/Cycles are : " $(cat one_caller_temp1.txt | grep -o '[\.0-9,]*  insns' | grep -o '[\.0-9,]*') 
#echo "Cache references are : " $(cat one_caller_temp1.txt | grep -o '[0-9,]* cache-references' | grep -o '[0-9,]*') 
#echo "cache-misses are : " $(cat one_caller_temp1.txt | grep -o '[0-9,]* cache-misses' | grep -o '[0-9,]*') 
#echo "cache refs are : " $(cat one_caller_temp1.txt | grep -o '[\.0-9,]* % of all cache' | grep -o '[\.0-9,]*')
#echo "L1-dcache-loads are : " $(cat one_caller_temp1.txt | grep -o '[0-9,]* L1-dcache-loads '| grep -o '[0-9,]*' | head -1)
#echo "L1-dcache-loads-misses are : " $(cat one_caller_temp1.txt | grep -o '[0-9,]* L1-dcache-loads-misses'| grep -o '[0-9,]*' | head -1)
#echo "cache refs are : " $(cat one_caller_temp1.txt | grep -o '[\.0-9,]*% of all L1-dcache' | grep -o '[\.0-9,]*' | head -1)
#echo "seconds elapse are : " $(cat one_caller_temp1.txt | grep -o '[\.0-9,]* seconds time elapsed' | grep -o '[\.0-9,]*')

COMMAND=$(head -1 one_caller_temp1.txt)
CYCLES=$(cat one_caller_temp1.txt | grep -o '[0-9,]* cycles' | grep -o '[0-9,]*')
INS=$(cat one_caller_temp1.txt | grep -o '[0-9,]* instructions' | grep -o '[0-9,]*')
INSPC=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]*  insns' | grep -o '[\.0-9,]*')
CACHEREF=$(cat one_caller_temp1.txt | grep -o '[0-9,]* cache-references' | grep -o '[0-9,]*')
CACHEMISS=$(cat one_caller_temp1.txt | grep -o '[0-9,]* cache-misses' | grep -o '[0-9,]*')
CACHEREFPCNT=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]* % of all cache' | grep -o '[\.0-9,]*')
L1CACHELOAD=$(cat one_caller_temp1.txt | grep -o '[0-9,]* L1-dcache-loads '| grep -o '[0-9,]*' | head -1)
L1CACHEMISS=$(cat one_caller_temp1.txt | grep -o '[0-9,]* L1-dcache-loads-misses'| grep -o '[0-9,]*' | head -1)
L1CACHEREFPCNT=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]*% of all L1-dcache' | grep -o '[\.0-9,]*' | head -1)
TOTALSECONDS=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]* seconds time elapsed' | grep -o '[\.0-9,]*')

echo $COMMAND"|"$CYCLES"|"$INS"|"$INSPC"|"$CACHEREF"|"$CACHEMISS"|"$CACHEREFPCNT"|"$L1CACHELOAD"|"$L1CACHEMISS"|"$L1CACHEREFPCNT"|"$TOTALSECONDS

rm -f one_caller_temp1.txt
