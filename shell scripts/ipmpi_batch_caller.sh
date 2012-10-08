#!/bin/bash
rm -f output.txt
./one_caller.sh './floattransposeIP -n 8192 -openmp -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 16384 -openmp -noio' >> output.txt
./one_caller.sh './floattransposeIP -n 32768 -openmp -noio' >> output.txt


cat output.txt | grep Performance > better_output.txt
