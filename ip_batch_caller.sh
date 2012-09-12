#!/bin/bash
rm -f output.txt
#./one_caller.sh './ipTranspose -n 512 -basic -noio' >> output.txt
#./one_caller.sh './ipTranspose -n 1024 -basic -noio' >> output.txt
#./one_caller.sh './ipTranspose -n 4096 -basic -noio' >> output.txt
#./one_caller.sh './ipTranspose -n 8192 -basic -noio' >> output.txt
#./one_caller.sh './ipTranspose -n 16384 -basic -noio' >> output.txt
#./one_caller.sh './ipTranspose -n 32256 -basic -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -1tiled -s1 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -1tiled -s1 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -1tiled -s1 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -1tiled -s1 32 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 4 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 4 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 8 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 8 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 8 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 16 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 16 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 16 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 16 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 32 -s2 2 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 32 -s2 4 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 32 -s2 8 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -2tiled -s1 32 -s2 16 -noio' >> output.txt
./one_caller.sh './ipTranspose -n 512 -cacheob -noio' >> output.txt
./one_caller.sh './ipTranspose -n 1024 -cacheob -noio' >> output.txt
./one_caller.sh './ipTranspose -n 4096 -cacheob -noio' >> output.txt
./one_caller.sh './ipTranspose -n 8192 -cacheob -noio' >> output.txt
./one_caller.sh './ipTranspose -n 16384 -cacheob -noio' >> output.txt
./one_caller.sh './ipTranspose -n 32256 -cacheob -noio' >> output.txt


cat output.txt | grep Performance > better_output.txt
