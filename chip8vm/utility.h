#ifndef C8VM_UTILITY_H
#define C8VM_UTILITY_H

#include <stdint.h>
#include <stddef.h>

uint64_t c8vm_utility_get_time_ms();
void *c8vm_utility_read_binary_file(const char *path, size_t *out_size);

#endif
