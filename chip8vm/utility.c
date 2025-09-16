#include "chip8vm/utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint64_t c8vm_utility_get_time_ms(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)ts.tv_nsec / 1000000ULL;
}

void *c8vm_utility_read_binary_file(const char *path, size_t *out_size)
{
	FILE *fp = fopen(path, "rb");

	if (!fp) {
		perror("fopen");
		return NULL;
	}

	if (fseek(fp, 0, SEEK_END) == -1) {
		perror("fseek");
		fclose(fp);
		return NULL;
	}

	long size = ftell(fp);

	if (size == -1) {
		perror("ftell");
		fclose(fp);
		return NULL;
	}

	if (size == 0) {
		fprintf(stderr, "c8vm_utility_read_binary_file: empty file: %s\n", path);
		fclose(fp);
		return NULL;
	}

	if (fseek(fp, 0, SEEK_SET) == -1) {
		perror("fseek");
		fclose(fp);
		return NULL;
	}

	void *buf = malloc((size_t)size);

	if (!buf) {
		perror("malloc");
		fclose(fp);
		return NULL;
	}

	if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
		perror("fread");
		free(buf);
		fclose(fp);
		return NULL;
	}

	fclose(fp);

	if (out_size) {
		*out_size = (size_t)size;
	}

	return buf;
}
