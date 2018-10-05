/*

To compile this program with gcc use:

gcc -fopenmp saxpypar.c -o saxpypar

#-------------------------------------------

*/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <arm_neon.h>

static long num_elements = 100000000;

void fill_array(float32_t* array)
{
	for (int i=0; i<num_elements; i++)
	{
		array[i] = (float32_t)(i)/rand();
	}
}

int main ()
{
	srand(time(0));
	float32_t alpha = 4.0;
	float32_t * x = malloc(sizeof(float32_t)*num_elements);
	float32_t * y = malloc(sizeof(float32_t)*num_elements);
	double start_time, run_time;
	fill_array(x);
	fill_array(y);
	
	omp_set_num_threads(4);
	/*start timer */
	start_time = omp_get_wtime();
	
	#pragma omp parallel for
	for (int i=0; i<(num_elements/4); i++)
	{
		float32x4_t xi = vld1q_f32(x);
		float32x4_t yi = vld1q_f32(y);
		yi = vaddq_f32(vmulq_n_f32(xi,alpha),yi);
		x+=4;
		y+=4;
	}
	
	run_time = omp_get_wtime() - start_time;
	printf("The operation took %f seconds \n",run_time);
	
}	
