#!/bin/bash

if [[ "$1" = "" ]] ; then
	echo "Argument missing"
	exit
fi

C1="perf stat -e cycles,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,page-faults,minor-faults,major-faults,cpu-migrations,context-switches -o temp_one_caller.txt $1"
$C1

tail -18 temp_one_caller.txt | sed 's/,//g' > one_caller_temp1.txt
rm -f temp_one_caller.txt

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
BRANCHES=$(cat one_caller_temp1.txt | grep -o '[0-9,]* branches' | grep -o '[0-9,]*')
BRANCHMISSES=$(cat one_caller_temp1.txt | grep -o '[0-9,]* branch-misses' | grep -o '[0-9,]*')
BRANCHMISSRATE=$(cat one_caller_temp1.txt | grep -o '[0-9,]*%of all branches' | grep -o '[0-9,]*')
INS=$(cat one_caller_temp1.txt | grep -o '[0-9,]* instructions' | grep -o '[0-9,]*')
INSPC=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]*  insns' | grep -o '[\.0-9,]*')
CACHEREF=$(cat one_caller_temp1.txt | grep -o '[0-9,]* cache-references' | grep -o '[0-9,]*')
CACHEMISS=$(cat one_caller_temp1.txt | grep -o '[0-9,]* cache-misses' | grep -o '[0-9,]*')
CACHEREFPCNT=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]* % of all cache' | grep -o '[\.0-9,]*')
L1CACHELOAD=$(cat one_caller_temp1.txt | grep -o '[0-9,]* L1-dcache-loads '| grep -o '[0-9,]*' | head -1)
L1CACHEMISS=$(cat one_caller_temp1.txt | grep -o '[0-9,]* L1-dcache-loads-misses'| grep -o '[0-9,]*' | head -1)
L1CACHEREFPCNT=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]*% of all L1-dcache' | grep -o '[\.0-9,]*' | head -1)
CPUMIGRATIONS=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]* cpu-migrations' | grep -o '[\.0-9,]*')
TOTALSECONDS=$(cat one_caller_temp1.txt | grep -o '[\.0-9,]* seconds time elapsed' | grep -o '[\.0-9,]*')
SCHEDULE=$(echo $OMP_SCHEDULE)
THREADS=$(echo $OMP_NUM_THREADS)

echo "stat_id|suite|"$COMMAND"|class|"$THREADS"|"$THREADS"|"$TOTALSECONDS"|mops|mopspersec|"$INS"|"$BRANCHES"|"$BRANCHMISSRATE"|"$CACHEREFPCNT"|"$L1CACHEREFPCNT"|"$INSPC"|"$CPUMIGRATIONS"|"$SCHEDULE"|"

#echo $COMMAND"|"$THREADS"|"$THREADS"|"$CYCLES"|"$INS"|"$INSPC"|"$CACHEREF"|"$CACHEMISS"|"$CACHEREFPCNT"|"$L1CACHELOAD"|"$L1CACHEMISS"|"$L1CACHEREFPCNT"|"$TOTALSECONDS"|"$SCHEDULE

rm -f one_caller_temp1.txt
