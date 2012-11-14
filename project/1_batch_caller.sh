#!/bin/bash
rm -f output.txt

# List of programs to run: 
# omptransposeIP omptransposeOP 
# compute_pi dot_product mandelbrot_openmp mxm mxv_openmp prime_openmp quad_openmp random_openmp

for program in omptransposeIP omptransposeOP; do

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
	for i in (1..32)
	do
	export OMP_NUM_THREADS=$i
	./one_caller.sh './omptransposeOP' >> output.txt
	done
	#
	#
	# Guided scheduling runs
	for i in (1..32)
	do
	export OMP_NUM_THREADS=$i
	export OMP_SCHEDULE="guided,4"
	./one_caller.sh './omptransposeOP' >> output.txt
	done

done

cat output.txt | grep Performance > better_output.txt
