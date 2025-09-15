#ifndef C8VM_VM_H
#define C8VM_VM_H

#include "chip8vm/memory.h"
#include "chip8vm/register.h"
#include "chip8vm/stack.h"
#include "chip8vm/keyboard.h"
#include "chip8vm/display.h"
#include "chip8vm/timer.h"

#define C8VM_CHARACTER_SET_SIZE 80
#define C8VM_CHARACTER_SET_HEIGHT 5
#define C8VM_CPU_CYCLES_PER_FRAME 10

struct c8vm_vm {
	struct c8vm_memory *c8vm_memory;
	struct c8vm_register *c8vm_register;
	struct c8vm_stack *c8vm_stack;
	struct c8vm_keyboard *c8vm_keyboard;
	struct c8vm_display *c8vm_display;
	struct c8vm_timer *c8vm_timer;
};

struct c8vm_vm *c8vm_vm_create();
void c8vm_vm_destroy(struct c8vm_vm *c8vm_vm);

bool c8vm_vm_load_rom(struct c8vm_vm *c8vm_vm, const char *rompath);
void c8vm_vm_cpu_cycle(struct c8vm_vm *c8vm_vm);

#endif
