#include "chip8vm/utility.h"

#include <time.h>

uint64_t c8vm_utility_get_time_ms()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}
