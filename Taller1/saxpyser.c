/*

To compile this program with gcc use:

gcc -fopenmp saxpyser.c -o saxpyser

*/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

static long num_elements = 100000000;

int main ()
{
	srand(time(0));
	float alpha = 4.0;
	float * x = malloc(sizeof(float)*num_elements);
	float * y = malloc(sizeof(float)*num_elements);
	double start_time, run_time;

	for (int i=0; i<num_elements; i++)
	{
		x[i] = (float)(i)/rand();
		y[i] = (float)(i)/rand();
	}

	/*start timer */
	start_time = omp_get_wtime();

	for (int i=0; i<num_elements; i++)
	{
		y[i] = alpha*x[i] + y[i];
	}
	
	run_time = omp_get_wtime() - start_time;
	printf("The operation took %f seconds \n",run_time);
	
}	
