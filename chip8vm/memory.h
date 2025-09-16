#ifndef C8VM_MEMORY_H
#define C8VM_MEMORY_H

#include <stdint.h>

/*
	Chip8 Memory Specifications
	Max Memory Size: 4096 Bytes
	Address 0x00-0x1FF: Reserved Reserved for Chip8 Internal
	Address 0x200-0xFFF: Reserved For Program Data
*/
#define C8VM_MEMORY_SIZE 0x1000
#define C8VM_MEMORY_INTERNAL_START 0x000
#define C8VM_MEMORY_INTERNAL_END 0x1FF
#define C8VM_MEMORY_PROGRAM_START 0x200
#define C8VM_MEMORY_PROGRAM_END 0xFFF

struct c8vm_memory {
	uint8_t memory[C8VM_MEMORY_SIZE];
};

struct c8vm_memory *c8vm_memory_create(void);
void c8vm_memory_destroy(struct c8vm_memory *c8vm_memory);

uint8_t c8vm_memory_read_uint8(const struct c8vm_memory *c8vm_memory, uint16_t offset);
void c8vm_memory_write_uint8(struct c8vm_memory *c8vm_memory, uint16_t offset, uint8_t data);

uint16_t c8vm_memory_read_uint16(const struct c8vm_memory *c8vm_memory, uint16_t offset);
void c8vm_memory_write_uint16(struct c8vm_memory *c8vm_memory, uint16_t offset, uint16_t data);

#endif
