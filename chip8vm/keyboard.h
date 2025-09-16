#ifndef C8VM_KEYBOARD_H
#define C8VM_KEYBOARD_H

#include <stdbool.h>
#include <stddef.h>

/*
	Chip8 Keyboard Specifications
	16-Key Hexadecimal Keyboard: 0-F
	Represented in 16-byte Bool Array (true=Pressed, false=Not Pressed)
	Remap The Chip8 Keys into PC Keys
*/
#define C8VM_KEYBOARD_KEY_SIZE 16
#define C8VM_KEYCODE_INVALID ((enum c8vm_keycode) - 1)
#define C8VM_ASCII_SIZE 128

enum c8vm_keycode {
	C8VM_KEY_0,	C8VM_KEY_1,	C8VM_KEY_2,	C8VM_KEY_3,
	C8VM_KEY_4,	C8VM_KEY_5,	C8VM_KEY_6,	C8VM_KEY_7,
	C8VM_KEY_8,	C8VM_KEY_9,	C8VM_KEY_A,	C8VM_KEY_B,
	C8VM_KEY_C,	C8VM_KEY_D,	C8VM_KEY_E,	C8VM_KEY_F,
};

struct c8vm_keyboard {
	bool remapped;
	bool key_state[C8VM_KEYBOARD_KEY_SIZE];
	enum c8vm_keycode key_map[C8VM_ASCII_SIZE];
};

struct c8vm_keyboard *c8vm_keyboard_create(void);
void c8vm_keyboard_destroy(struct c8vm_keyboard *c8vm_keyboard);

bool c8vm_keyboard_get_key_state(const struct c8vm_keyboard *c8vm_keyboard, int key);
void c8vm_keyboard_set_key_state(struct c8vm_keyboard *c8vm_keyboard, int key, bool state);

bool c8vm_keyboard_is_key_down(const struct c8vm_keyboard *c8vm_keyboard, enum c8vm_keycode keycode);
bool c8vm_keyboard_is_key_up(const struct c8vm_keyboard *c8vm_keyboard, enum c8vm_keycode keycode);

void c8vm_keyboard_remap(struct c8vm_keyboard *c8vm_keyboard, const enum c8vm_keycode *key_codes, const int *keys, size_t count);
enum c8vm_keycode c8vm_keyboard_get_mapped_keycode(const struct c8vm_keyboard *c8vm_keyboard, int key);

#endif
