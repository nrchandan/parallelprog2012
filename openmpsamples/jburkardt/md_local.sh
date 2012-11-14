#!/bin/bash
#
#  Compile the program with GCC.
#
gcc -fopenmp md_openmp.c -lm
#
#  Compile the program with ICC.
#
#icc -openmp -parallel md_openmp.c -lm
#
mv a.out md
#
#  Run with 1, 2, 4 and 8 threads.
#
echo "Run with 1 thread."
export OMP_NUM_THREADS=1
./md > md_local_output.txt
#
echo "Run with 2 threads."
export OMP_NUM_THREADS=2
./md >> md_local_output.txt
#
echo "Run with 4 threads."
export OMP_NUM_THREADS=4
./md >> md_local_output.txt
#
echo "Run with 8 threads."
export OMP_NUM_THREADS=8
./md >> md_local_output.txt
#
#  Discard the executable file.
#
rm md
#
echo "Program output written to md_local_output.txt"
