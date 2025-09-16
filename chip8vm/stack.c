#include "chip8vm/stack.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

struct c8vm_stack *c8vm_stack_create(void)
{
	struct c8vm_stack *c8vm_stack = malloc(sizeof(struct c8vm_stack));

	if (!c8vm_stack) {
		perror("malloc");
		return NULL;
	}

	memset(c8vm_stack, 0, sizeof(struct c8vm_stack));

	return c8vm_stack;
}

void c8vm_stack_destroy(struct c8vm_stack *c8vm_stack)
{
	free(c8vm_stack);
}

void c8vm_stack_push_uint16(struct c8vm_stack *c8vm_stack, struct c8vm_register *c8vm_register, uint16_t value)
{
	uint8_t sp = c8vm_register_read_sp(c8vm_register);
	assert(sp < C8VM_STACK_SIZE && "c8vm_stack_push16 stack overflow");

	c8vm_stack->stack[sp] = value;
	sp++;
	c8vm_register_write_sp(c8vm_register, sp);
}

uint16_t c8vm_stack_pop_uint16(struct c8vm_stack *c8vm_stack, struct c8vm_register *c8vm_register)
{
	uint8_t sp = c8vm_register_read_sp(c8vm_register);
	assert(sp > 0 && "c8vm_stack_pop16 stack underflow");

	sp--;
	uint16_t value = c8vm_stack->stack[sp];
	c8vm_register_write_sp(c8vm_register, sp);

	return value;
}

bool c8vm_stack_is_full(const struct c8vm_stack *c8vm_stack, const struct c8vm_register *c8vm_register)
{
	(void)c8vm_stack;
	uint8_t sp = c8vm_register_read_sp(c8vm_register);
	return sp >= C8VM_STACK_SIZE;
}

bool c8vm_stack_is_empty(const struct c8vm_stack *c8vm_stack, const struct c8vm_register *c8vm_register)
{
	(void)c8vm_stack;
	uint8_t sp = c8vm_register_read_sp(c8vm_register);
	return sp == 0;
}
