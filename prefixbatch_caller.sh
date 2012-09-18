#!/bin/bash
rm -f output.txt
./one_caller.sh './prefixSum -n 268435456 -basic -noio -noinit' >> output.txt
./one_caller.sh './prefixSum -n 1073741824 -basic -noio -noinit' >> output.txt
./one_caller.sh './prefixSum -n 268435456 -cacheob -noio -noinit' >> output.txt
./one_caller.sh './prefixSum -n 1073741824 -cacheob -noio -noinit' >> output.txt


cat output.txt | grep Performance > better_output.txt
