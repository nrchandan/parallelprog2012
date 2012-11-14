#!/bin/bash
rm -f output.txt

# List of programs to run: 
# In first run: omptransposeIP omptransposeOP 
# In this run: compute_pi dot_product mandelbrot_openmp mxm mxv_openmp prime_openmp quad_openmp random_openmp

for program in compute_pi dot_product mandelbrot_openmp mxm mxv_openmp prime_openmp quad_openmp random_openmp; do

	# Static scheduling runs
	export OMP_SCHEDULE="static"
	for i in {1..32}
	do
	export OMP_NUM_THREADS=$i
	./one_caller.sh "./$program" >> output.txt
	done
	#
	#
	# Dynamic scheduling runs
	export OMP_SCHEDULE="dynamic"
	for i in {1..32}
	do
	export OMP_NUM_THREADS=$i
	./one_caller.sh "./$program" >> output.txt
	done
	#
	#
	# Guided scheduling runs
	export OMP_SCHEDULE="guided,4"
	for i in {1..32}
	do
	export OMP_NUM_THREADS=$i
	./one_caller.sh "./$program" >> output.txt
	done

done

cat output.txt | grep Performance > better_output.txt
