#ifndef _CLOCKS_H_
#define _CLOCKS_H_

#include<stdint.h>
#include<stdlib.h>

#ifdef __INTEL_COMPILER
#define BARRIER __memory_barrier()
#else
#define BARRIER __asm__ __volatile__ ("" ::: "memory")
#endif

#define CLOCKS_RANDOM(RANDOM,FUNCTION) \
do{ \
	uint64_t start,end;int64_t i_bench,j_bench; \
	unsigned cycles_high0,cycles_low0; \
	unsigned cycles_high1,cycles_low1;  \
	__asm__ __volatile__ (\
	"mfence\n\t" \
	"RDTSC\n\t" \
	"mov %%edx, %0\n\t" \
	"mov %%eax, %1\n\t": "=r" (cycles_high0), "=r" (cycles_low0)::  \
	"%rax", "%rbx", "%rcx", "%rdx"); \
	BARRIER;\
	i_bench=BENCH;\
	do{\
		j_bench=BENCH;\
		RANDOM;\
		do{\
			FUNCTION;\
			j_bench--;\
		}while( j_bench != 0 );\
		i_bench--;\
	}while( i_bench != 0 );\
	BARRIER;\
	__asm__ __volatile__("RDTSCP\n\t"  \
	"mov %%edx, %0\n\t"   \
	"mov %%eax, %1\n\t"   \
	"mfence\n\t" \
	: "=r" (cycles_high1), "=r" (cycles_low1):: \
	"%rax", "%rbx", "%rcx", "%rdx"); \
	start= (((uint64_t)cycles_high0)<<32)|cycles_low0;\
	end  = (((uint64_t)cycles_high1)<<32)|cycles_low1;\
	printf("%s: %lu cc\n",#FUNCTION,(end-start)/(BENCH*BENCH));\
}while(0)

#define CLOCKS(FUNCTION) CLOCKS_RANDOM(while(0),FUNCTION)

/**
 * Taken from
 * agl/curve25519-donna
 * https://github.com/agl/curve25519-donna/blob/master/speed-curve25519.c
 *
 * ticks - not tested on anything other than x86
 * */
#define oper_second(RANDOM,FUNCTION) \
do{\
	printf("Operations per second: %s.\n",#FUNCTION);\
	RANDOM;\
\
	unsigned i;\
	uint64_t start, end;\
	const unsigned iterations = 100000;\
	uint64_t start_c, end_c;\
\
	/* Load the caches*/\
	for (i = 0; i < 1000; ++i) {\
	     FUNCTION ;\
	}\
\
	start = time_now();\
	start_c = cycles_now();\
	for (i = 0; i < iterations; ++i) {\
		FUNCTION; \
	}\
	end = time_now();\
	end_c = cycles_now();\
\
	printf("%lu us, %g op/s, %lu cycles/op\n",\
			(unsigned long) ((end - start) / iterations),\
			iterations*1000000. / (end - start),\
			(unsigned long)((end_c-start_c)/iterations) );\
}while(0)

uint64_t time_now();
uint64_t cycles_now(void);

/* _CLOCKS_H_ */ 
#endif
