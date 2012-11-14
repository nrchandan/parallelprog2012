#!/bin/bash
#
#  Compile the program with GCC.
#
gcc -fopenmp prime_openmp.c
#
#  Compile the program with ICC.
#
#icc -openmp -parallel prime_openmp.c
#
mv a.out prime
#
#  Run with 1, 2, 4 and 8 threads.
#
echo "Run with 20 threads."
echo "Run with 20 threads." > perf_prime_threads.txt
export OMP_NUM_THREADS=20
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 21 threads."
echo "Run with 21 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=21
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 22 threads."
echo "Run with 22 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=22
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 23 threads."
echo "Run with 23 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=23
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 24 threads."
echo "Run with 24 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=24
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 25 threads."
echo "Run with 25 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=25
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 26 threads."
echo "Run with 26 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=26
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 27 threads."
echo "Run with 27 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=27
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 28 threads."
echo "Run with 28 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=28
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
echo "Run with 29 threads."
echo "Run with 29 threads." >> perf_prime_threads.txt
export OMP_NUM_THREADS=29
perf stat -r 5 -o perf_prime_threads.tmp ./prime > perf_prime_threads.out
cat perf_prime_threads.tmp >> perf_prime_threads.txt
#
rm perf_prime_threads.tmp
#
#  Discard the executable file.
#
rm prime
#
echo "Program output written to prime_local_output.txt"
