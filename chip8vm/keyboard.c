#include "chip8vm/keyboard.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>

struct c8vm_keyboard *c8vm_keyboard_create(void)
{
	struct c8vm_keyboard *c8vm_keyboard = malloc(sizeof(struct c8vm_keyboard));

	if (!c8vm_keyboard) {
		perror("malloc");
		return NULL;
	}

	c8vm_keyboard->remapped = false;
	memset(c8vm_keyboard->key_state, 0, sizeof(c8vm_keyboard->key_state));

	for (size_t i = 0; i < C8VM_ASCII_SIZE; i++) {
		c8vm_keyboard->key_map[i] = C8VM_KEYCODE_INVALID;
	}

	// Map Default Keys
	c8vm_keyboard->key_map['0'] = C8VM_KEY_0;
	c8vm_keyboard->key_map['1'] = C8VM_KEY_1;
	c8vm_keyboard->key_map['2'] = C8VM_KEY_2;
	c8vm_keyboard->key_map['3'] = C8VM_KEY_3;
	c8vm_keyboard->key_map['4'] = C8VM_KEY_4;
	c8vm_keyboard->key_map['5'] = C8VM_KEY_5;
	c8vm_keyboard->key_map['6'] = C8VM_KEY_6;
	c8vm_keyboard->key_map['7'] = C8VM_KEY_7;
	c8vm_keyboard->key_map['8'] = C8VM_KEY_8;
	c8vm_keyboard->key_map['9'] = C8VM_KEY_9;
	c8vm_keyboard->key_map['a'] = C8VM_KEY_A;
	c8vm_keyboard->key_map['b'] = C8VM_KEY_B;
	c8vm_keyboard->key_map['c'] = C8VM_KEY_C;
	c8vm_keyboard->key_map['d'] = C8VM_KEY_D;
	c8vm_keyboard->key_map['e'] = C8VM_KEY_E;
	c8vm_keyboard->key_map['f'] = C8VM_KEY_F;

	return c8vm_keyboard;
}

void c8vm_keyboard_destroy(struct c8vm_keyboard *c8vm_keyboard)
{
	free(c8vm_keyboard);
}

bool c8vm_keyboard_get_key_state(const struct c8vm_keyboard *c8vm_keyboard, int key)
{
	if (key < 0 || key >= C8VM_ASCII_SIZE) {
		fprintf(stderr, "c8vm_keyboard_get_key_state key out of bounds: %d\n", key);
		return false;
	}

	key = tolower((unsigned char)key);

	enum c8vm_keycode keycode = c8vm_keyboard->key_map[key];

	if (keycode == C8VM_KEYCODE_INVALID || keycode >= C8VM_KEYBOARD_KEY_SIZE) {
		fprintf(stderr, "c8vm_keyboard_get_key_state key not mapped: %d\n", key);
		return false;
	}

	return c8vm_keyboard->key_state[(int)keycode];
}

void c8vm_keyboard_set_key_state(struct c8vm_keyboard *c8vm_keyboard, int key, bool state)
{
	if (key < 0 || key >= C8VM_ASCII_SIZE) {
		fprintf(stderr, "c8vm_keyboard_set_key_state key out of bounds: %d\n", key);
		return;
	}

	key = tolower((unsigned char)key);

	enum c8vm_keycode keycode = c8vm_keyboard->key_map[key];

	if (keycode == C8VM_KEYCODE_INVALID || keycode >= C8VM_KEYBOARD_KEY_SIZE) {
		fprintf(stderr, "c8vm_keyboard_set_key_state key not mapped: %d\n", key);
		return;
	}

	c8vm_keyboard->key_state[(int)keycode] = state;
}

bool c8vm_keyboard_is_key_down(const struct c8vm_keyboard *c8vm_keyboard, enum c8vm_keycode keycode)
{
	assert((keycode != C8VM_KEYCODE_INVALID && keycode < C8VM_KEYBOARD_KEY_SIZE) && "c8vm_keyboard_is_key keycode out of bounds");
	return c8vm_keyboard->key_state[(int)keycode];
}

bool c8vm_keyboard_is_key_up(const struct c8vm_keyboard *c8vm_keyboard, enum c8vm_keycode keycode)
{
	return !c8vm_keyboard_is_key_down(c8vm_keyboard, keycode);
}

void c8vm_keyboard_remap(struct c8vm_keyboard *c8vm_keyboard, const enum c8vm_keycode *key_codes, const int *keys, size_t count)
{
	for (size_t i = 0; i < C8VM_ASCII_SIZE; i++) {
		c8vm_keyboard->key_map[i] = C8VM_KEYCODE_INVALID;
	}

	for (size_t i = 0; i < count; i++) {
		int key = tolower((unsigned char)keys[i]);
		assert((key >= 0 && key < C8VM_ASCII_SIZE) && "c8vm_keyboard_remap key out of bounds");
		enum c8vm_keycode keycode = key_codes[i];
		assert((keycode != C8VM_KEYCODE_INVALID && keycode < C8VM_KEYBOARD_KEY_SIZE) && "c8vm_keyboard_remap keycode out of bounds");
		c8vm_keyboard->key_map[key] = keycode;
	}

	c8vm_keyboard->remapped = true;
}

enum c8vm_keycode c8vm_keyboard_get_mapped_keycode(const struct c8vm_keyboard *c8vm_keyboard, int key)
{
	key = tolower((unsigned char)key);

	if (key < 0 || key >= C8VM_ASCII_SIZE) {
		fprintf(stderr, "c8vm_keyboard_get_mapped_keycode key out of bounds: %d\n", key);
		return C8VM_KEYCODE_INVALID;
	}

	enum c8vm_keycode keycode = c8vm_keyboard->key_map[key];

	if (keycode == C8VM_KEYCODE_INVALID || keycode >= C8VM_KEYBOARD_KEY_SIZE) {
		fprintf(stderr, "c8vm_keyboard_get_mapped_keycode key not mapped: %d\n", key);
		return C8VM_KEYCODE_INVALID;
	}

	return keycode;
}
