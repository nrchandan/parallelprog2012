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
export OMP_SCHEDULE='static'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm > perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp > perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,2'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,3'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,4'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,5'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,10'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,20'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,50'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
echo "Run with 14 threads."
export OMP_NUM_THREADS=14
export OMP_SCHEDULE='static,100'
perf stat -r 5 -o perf_mxm_guided_14.tmp ./mxm >> perf_mxm_guided_14.out
cat perf_mxm_guided_14.tmp >> perf_mxm_guided_14.txt
#
rm -f perf_mxm_guided_14.tmp
#
#. Discard the executable file.
#
rm mxm
#
#echo "Program output written to perf_mxm_guided_14.out"
