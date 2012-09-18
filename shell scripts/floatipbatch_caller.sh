#!/bin/bash
rm -f output.txt
./one_caller.sh './floattransposeIP -n 8192 -basic -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -basic -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -basic -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -cacheob -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -cacheob -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -cacheob -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 8192 -mkl -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -mkl -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -mkl -noio' >> output.txt


cat output.txt | grep Performance > better_output.txt
