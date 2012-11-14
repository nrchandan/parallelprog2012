#!/bin/bash
#
#  Compile the program with GCC.
#
/usr/bin/gcc -fopenmp mxm.c -lm
#
#  Compile the program with ICC.
#
#icc -openmp -parallel mxm.c -lm
#
mv a.out mxm
#
#  Run with 1, 2, and 4 threads.
#
echo "Run with 12 threads."
export OMP_NUM_THREADS=12
perf stat -r 1 -o mxm_temp_local_output_perf.txt ./mxm > mxm_temp_local_output.txt
#
#echo "Run with 13 threads."
#export OMP_NUM_THREADS=13
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 14 threads."
#export OMP_NUM_THREADS=14
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 15 threads."
#export OMP_NUM_THREADS=15
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 16 threads."
#export OMP_NUM_THREADS=16
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 17 threads."
#export OMP_NUM_THREADS=17
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 18 threads."
#export OMP_NUM_THREADS=18
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 19 threads."
#export OMP_NUM_THREADS=19
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 20 threads."
#export OMP_NUM_THREADS=20
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 21 threads."
#export OMP_NUM_THREADS=21
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#echo "Run with 22 threads."
#export OMP_NUM_THREADS=22
#perf stat -r 5 ./mxm  >> mxm_local_output.txt
#
#  Discard the executable file.
#
rm mxm
#
#echo "Program output written to mxm_local_output.txt"
