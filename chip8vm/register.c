#include "chip8vm/register.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct c8vm_register *c8vm_register_create()
{
	struct c8vm_register *c8vm_register = malloc(sizeof(struct c8vm_register));

	if (!c8vm_register) {
		perror("malloc");
		return NULL;
	}

	memset(c8vm_register, 0, sizeof(struct c8vm_register));

	return c8vm_register;
}

void c8vm_register_destroy(struct c8vm_register *c8vm_register)
{
	free(c8vm_register);
}

uint8_t c8vm_register_read_v(const struct c8vm_register *c8vm_register, uint8_t vreg)
{
	assert(vreg < C8VM_REGISTER_GPR_SIZE);
	return c8vm_register->v[vreg];
}

void c8vm_register_write_v(struct c8vm_register *c8vm_register, uint8_t vreg, uint8_t data)
{
	assert(vreg < C8VM_REGISTER_GPR_SIZE);
	c8vm_register->v[vreg] = data;
}

uint16_t c8vm_register_read_i(const struct c8vm_register *c8vm_register)
{
	return c8vm_register->i;
}

void c8vm_register_write_i(struct c8vm_register *c8vm_register, uint16_t data)
{
	c8vm_register->i = data;
}

uint8_t c8vm_register_read_dt(const struct c8vm_register *c8vm_register)
{
	return c8vm_register->dt;
}

void c8vm_register_write_dt(struct c8vm_register *c8vm_register, uint8_t data)
{
	c8vm_register->dt = data;
}

uint8_t c8vm_register_read_st(const struct c8vm_register *c8vm_register)
{
	return c8vm_register->st;
}

void c8vm_register_write_st(struct c8vm_register *c8vm_register, uint8_t data)
{
	c8vm_register->st = data;
}

uint16_t c8vm_register_read_pc(const struct c8vm_register *c8vm_register)
{
	return c8vm_register->pc;
}

void c8vm_register_write_pc(struct c8vm_register *c8vm_register, uint16_t data)
{
	c8vm_register->pc = data;
}

uint8_t c8vm_register_read_sp(const struct c8vm_register *c8vm_register)
{
	return c8vm_register->sp;
}

void c8vm_register_write_sp(struct c8vm_register *c8vm_register, uint8_t data)
{
	c8vm_register->sp = data;
}
