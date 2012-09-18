#!/bin/bash
rm -f output.txt
./one_caller.sh './basictransposeOP0 -m 1024 -n 1024 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP0 -m 4096 -n 4096 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP0 -m 8192 -n 8192 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP0 -m 16384 -n 16384 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP0 -m 32768 -n 32768 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP1 -m 1024 -n 1024 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP1 -m 4096 -n 4096 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP1 -m 8192 -n 8192 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP1 -m 16384 -n 16384 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP1 -m 32768 -n 32768 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP2 -m 1024 -n 1024 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP2 -m 4096 -n 4096 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP2 -m 8192 -n 8192 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP2 -m 16384 -n 16384 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP2 -m 32768 -n 32768 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP3 -m 1024 -n 1024 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP3 -m 4096 -n 4096 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP3 -m 8192 -n 8192 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP3 -m 16384 -n 16384 -basic -noio -noinit' >> output.txt
./one_caller.sh './basictransposeOP3 -m 32768 -n 32768 -basic -noio -noinit' >> output.txt

cat output.txt | grep Performance > better_output.txt
