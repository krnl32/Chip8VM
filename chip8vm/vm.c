#include "chip8vm/vm.h"

#include <stdlib.h>
#include <string.h>

static const uint8_t c8vm_default_character_set[C8VM_CHARACTER_SET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80 // F
};

struct c8vm_vm *c8vm_vm_create()
{
	struct c8vm_vm *c8vm_vm = malloc(sizeof(struct c8vm_vm));

	if (!c8vm_vm) {
		perror("malloc");
		return NULL;
	}

	c8vm_vm->c8vm_memory = c8vm_memory_create();
	c8vm_vm->c8vm_register = c8vm_register_create();
	c8vm_vm->c8vm_stack = c8vm_stack_create();
	c8vm_vm->c8vm_keyboard = c8vm_keyboard_create();
	c8vm_vm->c8vm_display = c8vm_display_create();
	c8vm_vm->c8vm_timer = c8vm_timer_create();

	// Copy Default C8VM Character Set
	memcpy(c8vm_vm->c8vm_memory->memory, c8vm_default_character_set, C8VM_CHARACTER_SET_SIZE);

	return c8vm_vm;
}

void c8vm_vm_destroy(struct c8vm_vm *c8vm_vm)
{
	if (c8vm_vm) {
		c8vm_timer_destroy(c8vm_vm->c8vm_timer);
		c8vm_display_destroy(c8vm_vm->c8vm_display);
		c8vm_keyboard_destroy(c8vm_vm->c8vm_keyboard);
		c8vm_stack_destroy(c8vm_vm->c8vm_stack);
		c8vm_register_destroy(c8vm_vm->c8vm_register);
		c8vm_memory_destroy(c8vm_vm->c8vm_memory);

		free(c8vm_vm);
	}
}
