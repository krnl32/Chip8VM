#ifndef C8VM_STACK_H
#define C8VM_STACK_H

#include "chip8vm/register.h"

#include <stdbool.h>

/*
	Chip8 Stack Specifications
	Stack: Array of 16, 16-Bit Values
	Stores Function Return Address
	Max 16 Recursion Calls
	Stack is Separate from Main Memory
*/
#define C8VM_STACK_SIZE 16

struct c8vm_stack {
	uint16_t stack[C8VM_STACK_SIZE];
};

struct c8vm_stack *c8vm_stack_create(void);
void c8vm_stack_destroy(struct c8vm_stack *c8vm_stack);

void c8vm_stack_push_uint16(struct c8vm_stack *c8vm_stack, struct c8vm_register *c8vm_register, uint16_t value);
uint16_t c8vm_stack_pop_uint16(struct c8vm_stack *c8vm_stack, struct c8vm_register *c8vm_register); /* return address at top of stack or 0 for errors */
bool c8vm_stack_is_full(const struct c8vm_stack *c8vm_stack, const struct c8vm_register *c8vm_register);
bool c8vm_stack_is_empty(const struct c8vm_stack *c8vm_stack, const struct c8vm_register *c8vm_register);

#endif
