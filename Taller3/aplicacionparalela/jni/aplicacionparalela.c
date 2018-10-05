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
#include <arm_neon.h>

#define PI 3.14159265

static long num_sines = 10000000;

void fill_arrays(int32_t* indexes, float32_t *sins, float x)
{   
	#pragma omp parallel for
    for (int i = 1; i <= num_sines; i++)
    {
		indexes[i] = i;
		sins[i] = sin(((i*2)-1)*x);
    }
}

int main ()
{
	float x = 2.5;
	int nprocs;
	int32x4_t veci, ix2, ix2m1;

	float32x4_t n, dix2m1, sinix2m1;
	float32x4_t tof32x4;
	int32x4_t to32x4;
	
	double start_time, run_time;
	int32_t *indexes = malloc(sizeof(int32_t)*num_sines);
	float32_t *sins = malloc(sizeof(float32_t)*num_sines);
	float32x4_t sum = vdupq_n_f32(0.0);
	float32_t collapsedsum = 0.0;
	float f;
	float32_t* beforecollapse = malloc(sizeof(float32_t)*4);
	int32x4_t one = vdupq_n_s32(1);
	int32x4_t two = vdupq_n_s32(2);

	/* Use double of system processors (threads) */
	nprocs=2*omp_get_num_procs();
	omp_set_num_threads(nprocs);
	fill_arrays(indexes, sins, x);
	start_time = omp_get_wtime();	
	#pragma omp parallel for private(n)
	for (int i=0;i< num_sines; i+=4){
		veci = vld1q_s32(indexes);
		indexes += 4;

		ix2 = vmulq_s32(veci, two);
		ix2m1 = vsubq_s32(ix2, one);
		tof32x4 = vcvtq_f32_s32(ix2m1);
	 	dix2m1 = vrecpeq_f32(tof32x4);

		sinix2m1 = vld1q_f32(sins);
		sins += 4;

		n = vmulq_f32(dix2m1, sinix2m1);

		//n = (1.0/((i*2)-1))*sins[i];
		sum = vaddq_f32(sum, n);
	}
	vst1q_f32(beforecollapse, sum);
	#pragma omp parallel for reduction(+:collapsedsum)
	for (int i=0; i<4; i++)
	{
		collapsedsum = collapsedsum + beforecollapse[i];
	}

	f = (4.0/PI)*collapsedsum;
	run_time = omp_get_wtime() - start_time;
	printf("\n f(%f) is %f in %f seconds\n", x, f ,run_time);
}	  



