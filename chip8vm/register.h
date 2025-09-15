#ifndef C8VM_REGISTER_H
#define C8VM_REGISTER_H

#include <stdint.h>

/*
	Chip8 Register Specifications
	16 General Purpose 8-Bit Registers (Vx, x=0-F)
	16-Bit Register "I" for Storing Memory Address
	VF Register Is not a General-Purpose Registers, It's Is a Flag Register used by Certain Instructions
	2 Special Purpose 8-Bit Registers for Delay & Sound Timers. (When Non-Zero, Decrement at rate of 60Hz)
	16-Bit Program Counter(PC) Register for Storing Address of Current Instruction to Execute (Instruction is 2 Bytes in Size)
	8-Bit StackPointer (Points at Top of the Stack)
*/
#define C8VM_REGISTER_GPR_SIZE 16

struct c8vm_register {
	uint8_t v[C8VM_REGISTER_GPR_SIZE];
	uint16_t i;
	uint8_t dt;
	uint8_t st;
	uint16_t pc;
	uint8_t sp;
};

struct c8vm_register *c8vm_register_create(void);
void c8vm_register_destroy(struct c8vm_register *c8vm_register);

uint8_t c8vm_register_read_v(const struct c8vm_register *c8vm_register, uint8_t vreg);
void c8vm_register_write_v(struct c8vm_register *c8vm_register, uint8_t vreg, uint8_t data);

uint16_t c8vm_register_read_i(const struct c8vm_register *c8vm_register);
void c8vm_register_write_i(struct c8vm_register *c8vm_register, uint16_t data);

uint8_t c8vm_register_read_dt(const struct c8vm_register *c8vm_register);
void c8vm_register_write_dt(struct c8vm_register *c8vm_register, uint8_t data);

uint8_t c8vm_register_read_st(const struct c8vm_register *c8vm_register);
void c8vm_register_write_st(struct c8vm_register *c8vm_register, uint8_t data);

uint16_t c8vm_register_read_pc(const struct c8vm_register *c8vm_register);
void c8vm_register_write_pc(struct c8vm_register *c8vm_register, uint16_t data);

uint8_t c8vm_register_read_sp(const struct c8vm_register *c8vm_register);
void c8vm_register_write_sp(struct c8vm_register *c8vm_register, uint8_t data);

#endif
