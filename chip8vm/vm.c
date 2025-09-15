#include "chip8vm/vm.h"
#include "chip8vm/utility.h"
#include "chip8vm/platform.h"

#include <assert.h>
#include <stdio.h>
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

static void c8vm_vm_cpu_execute(struct c8vm_vm *c8vm_vm, uint16_t opcode);
static void c8vm_vm_cpu_skip_instruction(struct c8vm_vm *c8vm_vm);

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

	c8vm_register_write_pc(c8vm_vm->c8vm_register, C8VM_MEMORY_PROGRAM_START);

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

bool c8vm_vm_load_rom(struct c8vm_vm *c8vm_vm, const char *rompath)
{
	size_t rom_size = 0;
	uint8_t *rom = c8vm_utility_read_binary_file(rompath, &rom_size);

	if (!rom) {
		printf("c8vm_utility_read_binary_file(rompath) failed\n");
		return false;
	}

	if (rom_size + C8VM_MEMORY_PROGRAM_START > C8VM_MEMORY_SIZE) {
		fprintf(stderr, "ROM %s too large (%zu bytes) for C8VM\n", rompath, rom_size);
		free(rom);
		return false;
	}

	memcpy(c8vm_vm->c8vm_memory->memory + C8VM_MEMORY_PROGRAM_START, rom, rom_size);
	free(rom);

	printf("c8vm loaded ROM: %s (%zu bytes)\n", rompath, rom_size);
	return true;
}

void c8vm_vm_cpu_cycle(struct c8vm_vm *c8vm_vm)
{
	uint16_t pc = c8vm_register_read_pc(c8vm_vm->c8vm_register);

	if (pc + 1 >= C8VM_MEMORY_SIZE) {
		fprintf(stderr, "PC out of bounds: 0x%X\n", pc);
		return;
	}

	uint16_t opcode = c8vm_memory_read_uint16(c8vm_vm->c8vm_memory, pc);
#ifdef DEBUG
	printf("c8vm_vm_cpu_cycle executing opcode: 0x%04X at PC: 0x%03X\n", opcode, pc);
#endif
	c8vm_register_write_pc(c8vm_vm->c8vm_register, pc + 2);
	c8vm_vm_cpu_execute(c8vm_vm, opcode);
}

static void c8vm_vm_cpu_execute(struct c8vm_vm *c8vm_vm, uint16_t opcode)
{
	// Decode
	const uint16_t nnn = opcode & 0x0FFF;
	const uint8_t n = opcode & 0x000F;
	const uint8_t x = (opcode & 0x0F00) >> 8;
	const uint8_t y = (opcode & 0x00F0) >> 4;
	const uint8_t kk = opcode & 0x00FF;

	switch (opcode & 0xF000) {
		case 0x0000: {
			switch (opcode) {
					/* clear the display */
				case 0x00E0:
					c8vm_display_clear(c8vm_vm->c8vm_display);
					break;

					/* return from a subroutine */
				case 0x00EE: {
					uint16_t pc = c8vm_stack_pop_uint16(c8vm_vm->c8vm_stack, c8vm_vm->c8vm_register);
					c8vm_register_write_pc(c8vm_vm->c8vm_register, pc);
					break;
				}

				default:
					fprintf(stderr, "unknown 0x0nnn opcode: 0x%04X\n", opcode);
					break;
			}
			break;
		}

			/* jump to location nnn */
		case 0x1000:
			c8vm_register_write_pc(c8vm_vm->c8vm_register, nnn);
			break;

			/* call subroutine at nnn */
		case 0x2000: {
			uint16_t pc = c8vm_register_read_pc(c8vm_vm->c8vm_register);
			c8vm_stack_push_uint16(c8vm_vm->c8vm_stack, c8vm_vm->c8vm_register, pc);
			c8vm_register_write_pc(c8vm_vm->c8vm_register, nnn);
			break;
		}

			/* skip next instruction if Vx == kk */
		case 0x3000: {
			const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);

			if (vx == kk) {
				c8vm_vm_cpu_skip_instruction(c8vm_vm);
			}

			break;
		}

			/* skip next instruction if Vx != kk */
		case 0x4000: {
			const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);

			if (vx != kk) {
				c8vm_vm_cpu_skip_instruction(c8vm_vm);
			}

			break;
		}

		case 0x5000: {
			/* skip next instruction if Vx == Vy */
			if (n == 0x0) {
				const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
				const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);

				if (vx == vy) {
					c8vm_vm_cpu_skip_instruction(c8vm_vm);
				}
			} else {
				fprintf(stderr, "unknown 0x5xyn opcode: 0x%04X\n", opcode);
			}

			break;
		}

			/* set Vx = kk */
		case 0x6000:
			c8vm_register_write_v(c8vm_vm->c8vm_register, x, kk);
			break;

			/* set Vx = Vx + kk */
		case 0x7000: {
			const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
			uint8_t res = vx + kk;
			c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
			break;
		}

		case 0x8000: {
			switch (n) {
					/* set Vx = Vy */
				case 0x0: {
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, vy);
					break;
				}

					/* set Vx = Vx OR Vy */
				case 0x1: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					uint8_t res = vx | vy;
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

					/* set Vx = Vx AND Vy */
				case 0x2: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					uint8_t res = vx & vy;
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

					/* set Vx = Vx XOR Vy */
				case 0x3: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					uint8_t res = vx ^ vy;
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

					/* set Vx = Vx + Vy, set VF = carry */
				case 0x4: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					uint16_t res = vx + vy;
					uint8_t vf = (res > 255);

					c8vm_register_write_v(c8vm_vm->c8vm_register, 0xF, vf);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, (uint8_t)res);
					break;
				}

					/* set Vx = Vx - Vy, set VF = NOT borrow */
				case 0x5: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					uint8_t res = vx - vy;
					uint8_t vf = (vx > vy);

					c8vm_register_write_v(c8vm_vm->c8vm_register, 0xF, vf);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

					/* set Vx = Vx SHR 1 */
				case 0x6: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					uint8_t res = vx >> 1;
					uint8_t vf = (vx & 0x1);

					c8vm_register_write_v(c8vm_vm->c8vm_register, 0xF, vf);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

					/* set Vx = Vy - Vx, set VF = NOT borrow */
				case 0x7: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
					uint8_t res = vy - vx;
					uint8_t vf = (vy > vx);

					c8vm_register_write_v(c8vm_vm->c8vm_register, 0xF, vf);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

					/* set Vx = Vx SHL 1 */
				case 0xE: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					uint8_t res = vx << 1;
					uint8_t vf = (vx & 0x80);

					c8vm_register_write_v(c8vm_vm->c8vm_register, 0xF, vf);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
					break;
				}

				default:
					fprintf(stderr, "unknown 0x8xyn opcode: 0x%04X\n", opcode);
					break;
			}
			break;
		}

		case 0x9000: {
			/* skip next instruction if Vx != Vy */
			if (n == 0x0) {
				const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
				const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);

				if (vx != vy) {
					c8vm_vm_cpu_skip_instruction(c8vm_vm);
				}
			} else {
				fprintf(stderr, "unknown 0x9xyn opcode: 0x%04X\n", opcode);
			}

			break;
		}

			/* set I = nnn */
		case 0xA000:
			c8vm_register_write_i(c8vm_vm->c8vm_register, nnn);
			;
			break;

			/* jump to location nnn + V0 */
		case 0xB000: {
			const uint8_t v0 = c8vm_register_read_v(c8vm_vm->c8vm_register, 0x0);
			uint16_t res = v0 + nnn;
			c8vm_register_write_pc(c8vm_vm->c8vm_register, res);
			break;
		}

			/* set Vx = random byte AND kk */
		case 0xC000: {
			uint8_t random = rand() % 256;
			uint8_t res = random & kk;
			c8vm_register_write_v(c8vm_vm->c8vm_register, x, res);
			break;
		}

			/* display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision */
		case 0xD000: {
			const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
			const uint8_t vy = c8vm_register_read_v(c8vm_vm->c8vm_register, y);
			const uint16_t i = c8vm_register_read_i(c8vm_vm->c8vm_register);
			const uint8_t *sprite = (c8vm_vm->c8vm_memory->memory + i);
			uint8_t vf = c8vm_display_draw_sprite(c8vm_vm->c8vm_display, vx, vy, sprite, n);
			c8vm_register_write_v(c8vm_vm->c8vm_register, 0xF, vf);
			break;
		}

		case 0xE000: {
			switch (kk) {
					/* skip next instruction if key with the value of Vx is pressed */
				case 0x9E: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					char key = c8vm_keyboard_keycode_to_key(c8vm_vm->c8vm_keyboard, vx);

					if (c8vm_keyboard_is_key_down(c8vm_vm->c8vm_keyboard, key)) {
						c8vm_vm_cpu_skip_instruction(c8vm_vm);
					}

					break;
				}

					/* skip next instruction if key with the value of Vx is not pressed */
				case 0xA1: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					char key = c8vm_keyboard_keycode_to_key(c8vm_vm->c8vm_keyboard, vx);

					if (c8vm_keyboard_is_key_up(c8vm_vm->c8vm_keyboard, key)) {
						c8vm_vm_cpu_skip_instruction(c8vm_vm);
					}

					break;
				}

				default:
					fprintf(stderr, "unknown 0xExkk opcode: 0x%04X\n", opcode);
					break;
			}
			break;
		}

		case 0xF000: {
			switch (kk) {
					/* set Vx = delay timer value */
				case 0x07: {
					const uint8_t dt = c8vm_register_read_dt(c8vm_vm->c8vm_register);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, dt);
					break;
				}

					/* wait for a key press, store the value of the key in Vx */
				case 0x0A: {
					char key = c8vm_platform_wait_for_key_press();
					enum c8vm_keycode keycode = c8vm_keyboard_key_to_keycode(c8vm_vm->c8vm_keyboard, key);
					c8vm_register_write_v(c8vm_vm->c8vm_register, x, (uint8_t)keycode);
					break;
				}

					/* set delay timer = Vx */
				case 0x15: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					c8vm_register_write_dt(c8vm_vm->c8vm_register, vx);
					break;
				}

					/* set sound timer = Vx */
				case 0x18: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					c8vm_register_write_st(c8vm_vm->c8vm_register, vx);
					break;
				}

					/* set I = I + Vx */
				case 0x1E: {
					const uint16_t i = c8vm_register_read_i(c8vm_vm->c8vm_register);
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					uint16_t res = i + vx;
					c8vm_register_write_i(c8vm_vm->c8vm_register, res);
					break;
				}

					/* set I = location of sprite for digit Vx */
				case 0x29: {
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					uint16_t res = vx * C8VM_CHARACTER_SET_HEIGHT;
					c8vm_register_write_i(c8vm_vm->c8vm_register, res);
					break;
				}

					/* store BCD representation of Vx in memory locations I, I+1, and I+2 */
				case 0x33: {
					const uint16_t i = c8vm_register_read_i(c8vm_vm->c8vm_register);
					const uint8_t vx = c8vm_register_read_v(c8vm_vm->c8vm_register, x);
					uint8_t hundreds = vx / 100;
					uint8_t tens = (vx / 10) % 10;
					uint8_t ones = vx % 10;

					c8vm_memory_write_uint8(c8vm_vm->c8vm_memory, i, hundreds);
					c8vm_memory_write_uint8(c8vm_vm->c8vm_memory, i + 1, tens);
					c8vm_memory_write_uint8(c8vm_vm->c8vm_memory, i + 2, ones);
					break;
				}

					/* store registers V0 through Vx in memory starting at location I */
				case 0x55: {
					const uint16_t i = c8vm_register_read_i(c8vm_vm->c8vm_register);

					for (uint8_t j = 0; j <= x; j++) {
						uint8_t reg_data = c8vm_register_read_v(c8vm_vm->c8vm_register, j);
						c8vm_memory_write_uint8(c8vm_vm->c8vm_memory, i + j, reg_data);
					}

					break;
				}

					/* read registers V0 through Vx from memory starting at location I */
				case 0x65: {
					const uint16_t i = c8vm_register_read_i(c8vm_vm->c8vm_register);

					for (uint8_t j = 0; j <= x; j++) {
						uint8_t reg_data = c8vm_memory_read_uint8(c8vm_vm->c8vm_memory, i + j);
						c8vm_register_write_v(c8vm_vm->c8vm_register, j, reg_data);
					}

					break;
				}

				default:
					fprintf(stderr, "unknown 0xFxkk opcode: 0x%04X\n", opcode);
					break;
			}
			break;
		}
	}
}

static void c8vm_vm_cpu_skip_instruction(struct c8vm_vm *c8vm_vm)
{
	uint16_t pc = c8vm_register_read_pc(c8vm_vm->c8vm_register);
	pc += 2;
	c8vm_register_write_pc(c8vm_vm->c8vm_register, pc);
}
