#!/bin/bash
#
#  Compile with GCC.
#
gcc -fopenmp multitask_openmp.c
if [ $? -ne 0 ]; then
  echo "Errors compiling multitask_openmp.c"
  exit
fi
#
mv a.out multitask
#
#  Request 1 thread.
#
echo "Run with 1 thread."
export OMP_NUM_THREADS=1
./multitask > multitask_local_output.txt
#
#  Request 2 threads.
#
echo "Run with 2 threads."
export OMP_NUM_THREADS=2
./multitask >> multitask_local_output.txt
#
#  Request 4 threads.
#
echo "Run with 4 threads."
export OMP_NUM_THREADS=4
./multitask >> multitask_local_output.txt
#
#  Discard the executable.
#
rm multitask
#
echo "Program output written to multitask_local_output.txt"
