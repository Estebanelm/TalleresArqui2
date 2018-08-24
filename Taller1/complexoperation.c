/*

This program will calculate the image of x for the function

f(x) = 1 using the Fourier Sine series. The result should always be

1 no matter the input, but the precision of this number 1 depends

on the amount of sines calculated and added to the function.

The series used is taken from:
http://www.sosmath.com/fourier/fourier2/fourier2.html

To compile this program with gcc use:
gcc -fopenmp complexoperation.c -o complexoperation -lm

#-----------------------------------------------------------------

*/

#include <stdio.h>
#include <omp.h>
#include <math.h>

#define PI 3.14159265

static long num_sines = 10000000;
double step;
int main ()
{
	float x = 2.5;
	float f, n;
	float sum = 0;
	int i, nprocs;
	double start_time, run_time;

	step = 1.0/(double) num_sines;
	/* Use double of system processors (threads) */
	nprocs=2*omp_get_num_procs();
	sum = 0.0;
	omp_set_num_threads(nprocs);
	start_time = omp_get_wtime();
	#pragma omp parallel for reduction(+:sum) private(n)
	for (i=1;i<= num_sines; i++){
		n = (1.0/((i*2)-1))*sin(((i*2)-1)*x);
		sum = sum + n;
	}

	f = (4.0/PI)*sum;	
	run_time = omp_get_wtime() - start_time;
	printf("\n f(%f) is %f in %f seconds and %d threads\n", x, f ,run_time, nprocs);
}	  


