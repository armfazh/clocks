#include "clocks.h"
#include <sys/time.h>
#include <time.h>

uint64_t time_now() 
{
	struct timeval tv;
	uint64_t ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_sec;
	ret *= 1000000;
	ret += tv.tv_usec;

	return ret;
}

uint64_t cycles_now(void) {
#if defined(__GNUC__)
	uint32_t lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)lo | ((uint64_t)hi << 32));
#else
		return 0;	/* Undefined for now; should be obvious in the output */
        #endif
}


