#include "chip8vm/memory.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct c8vm_memory *c8vm_memory_create()
{
	struct c8vm_memory *c8vm_memory = malloc(sizeof(struct c8vm_memory));

	if (!c8vm_memory) {
		perror("malloc");
		return NULL;
	}

	memset(c8vm_memory->memory, 0, sizeof(c8vm_memory->memory));

	return c8vm_memory;
}

void c8vm_memory_destroy(struct c8vm_memory *c8vm_memory)
{
	free(c8vm_memory);
}

uint8_t c8vm_memory_read_uint8(const struct c8vm_memory *c8vm_memory, uint16_t offset)
{
	assert(offset < C8VM_MEMORY_SIZE);
	return c8vm_memory->memory[offset];
}

void c8vm_memory_write_uint8(struct c8vm_memory *c8vm_memory, uint16_t offset, uint8_t data)
{
	assert(offset < C8VM_MEMORY_SIZE);
	c8vm_memory->memory[offset] = data;
}

uint16_t c8vm_memory_read_uint16(const struct c8vm_memory *c8vm_memory, uint16_t offset)
{
	assert(offset <= C8VM_MEMORY_SIZE - 2);
	uint8_t high = c8vm_memory->memory[offset];
	uint8_t low = c8vm_memory->memory[offset + 1];
	return ((uint16_t)high << 8) | low;
}

void c8vm_memory_write_uint16(struct c8vm_memory *c8vm_memory, uint16_t offset, uint16_t data)
{
	assert(offset <= C8VM_MEMORY_SIZE - 2);
	uint8_t high = (data >> 8) & 0xFF;
	uint8_t low = data & 0xFF;
	c8vm_memory->memory[offset] = high;
	c8vm_memory->memory[offset + 1] = low;
}
