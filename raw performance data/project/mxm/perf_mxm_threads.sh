#./bin/bash
#
#. Compile the program with GCC.
#
/usr/bin/gcc -fopenmp mxm.c -lm
#
#. Compile the program with ICC.
#
#.cc -openmp -parallel mxm.c -lm
#
mv a.out mxm
#
#. Run with 1, 2, and 4 threads.
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
perf stat -r 5 -o perf_mxm_threads.tmp ./mxm > perf_mxm_threads.out
cat perf_mxm_threads.tmp > perf_mxm_threads.txt
#
echo "Run with 16 threads."
export OMP_NUM_THREADS=16
perf stat -r 5 -o perf_mxm_threads.tmp ./mxm >> perf_mxm_threads.out
cat perf_mxm_threads.tmp >> perf_mxm_threads.txt
#
echo "Run with 18 threads."
export OMP_NUM_THREADS=18
perf stat -r 5 -o perf_mxm_threads.tmp ./mxm >> perf_mxm_threads.out
cat perf_mxm_threads.tmp >> perf_mxm_threads.txt
#
#. Discard the executable file.
#
rm mxm
#
#echo "Program output written to perf_mxm_threads.txt"
