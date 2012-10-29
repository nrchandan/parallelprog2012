#!/bin/bash
#
#  Compile the program with GCC.
#
/usr/local/bin/gcc -fopenmp dot_product.c -lm
#
#  Compile the program with ICC.
#
#icc -openmp -parallel dot_product.c -lm
#
mv a.out dot_product
#
#  Run with 1, 2, and 4 threads.
#
echo "Run with 1 thread."
export OMP_NUM_THREADS=1
./dot_product > dot_product_local_output.txt
#
echo "Run with 2 threads."
export OMP_NUM_THREADS=2
./dot_product >> dot_product_local_output.txt
#
echo "Run with 4 threads."
export OMP_NUM_THREADS=4
./dot_product >> dot_product_local_output.txt
#
#  Discard the executable file.
#
rm dot_product
#
echo "Program output written to dot_product_local_output.txt"
