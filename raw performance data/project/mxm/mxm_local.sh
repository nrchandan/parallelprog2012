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
perf stat -r 5 ./mxm > mxm_local_output.txt
#
echo "Run with 16 threads."
export OMP_NUM_THREADS=16
perf stat -r 5 ./mxm >> mxm_local_output.txt
#
echo "Run with 18 threads."
export OMP_NUM_THREADS=18
perf stat -r 5 ./mxm >> mxm_local_output.txt
#
#. Discard the executable file.
#
rm mxm
#
#echo "Program output written to mxm_local_output.txt"
