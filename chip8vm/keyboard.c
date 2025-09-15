#include "chip8vm/keyboard.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

struct c8vm_keyboard *c8vm_keyboard_create()
{
	struct c8vm_keyboard *c8vm_keyboard = malloc(sizeof(struct c8vm_keyboard));

	if (!c8vm_keyboard) {
		perror("malloc");
		return NULL;
	}

	c8vm_keyboard->remapped = false;
	memset(c8vm_keyboard->key_state, 0, sizeof(c8vm_keyboard->key_state));
	memset(c8vm_keyboard->key_map, C8VM_KEYCODE_INVALID, sizeof(c8vm_keyboard->key_map));

	// Map Default Keys
	c8vm_keyboard->key_map['1'] = C8VM_KEY_1;
	c8vm_keyboard->key_map['2'] = C8VM_KEY_2;
	c8vm_keyboard->key_map['3'] = C8VM_KEY_3;
	c8vm_keyboard->key_map['c'] = C8VM_KEY_C;
	c8vm_keyboard->key_map['4'] = C8VM_KEY_4;
	c8vm_keyboard->key_map['5'] = C8VM_KEY_5;
	c8vm_keyboard->key_map['d'] = C8VM_KEY_D;
	c8vm_keyboard->key_map['7'] = C8VM_KEY_7;
	c8vm_keyboard->key_map['8'] = C8VM_KEY_8;
	c8vm_keyboard->key_map['9'] = C8VM_KEY_9;
	c8vm_keyboard->key_map['e'] = C8VM_KEY_E;
	c8vm_keyboard->key_map['a'] = C8VM_KEY_A;
	c8vm_keyboard->key_map['0'] = C8VM_KEY_0;
	c8vm_keyboard->key_map['b'] = C8VM_KEY_B;
	c8vm_keyboard->key_map['f'] = C8VM_KEY_F;

	return c8vm_keyboard;
}

void c8vm_keyboard_destroy(struct c8vm_keyboard *c8vm_keyboard)
{
	free(c8vm_keyboard);
}

bool c8vm_keyboard_get_key_state(const struct c8vm_keyboard *c8vm_keyboard, char key)
{
	char lower = tolower(key);
	enum c8vm_keycode key_code = c8vm_keyboard->key_map[(int)lower];

	assert(key_code != C8VM_KEYCODE_INVALID);
	assert(key_code < C8VM_KEYBOARD_KEY_SIZE);

	return c8vm_keyboard->key_state[(int)key_code];
}

void c8vm_keyboard_set_key_state(struct c8vm_keyboard *c8vm_keyboard, char key, bool state)
{
	char lower = tolower(key);
	enum c8vm_keycode key_code = c8vm_keyboard->key_map[(int)lower];

	assert(key_code != C8VM_KEYCODE_INVALID);
	assert(key_code < C8VM_KEYBOARD_KEY_SIZE);

	c8vm_keyboard->key_state[(int)key_code] = state;
}

bool c8vm_keyboard_is_key_down(const struct c8vm_keyboard *c8vm_keyboard, char key)
{
	return c8vm_keyboard_get_key_state(c8vm_keyboard, key);
}

bool c8vm_keyboard_is_key_up(const struct c8vm_keyboard *c8vm_keyboard, char key)
{
	return !c8vm_keyboard_is_key_down(c8vm_keyboard, key);
}

void c8vm_keyboard_remap(struct c8vm_keyboard *c8vm_keyboard, const enum c8vm_keycode *key_codes, const char *keys, size_t count)
{
	c8vm_keyboard->remapped = true;

	memset(c8vm_keyboard->key_map, C8VM_KEYCODE_INVALID, sizeof(c8vm_keyboard->key_map));

	for (int i = 0; i < count; i++) {
		int key = tolower(keys[i]);
		c8vm_keyboard->key_map[key] = key_codes[i];
	}
}

enum c8vm_keycode c8vm_keyboard_key_to_keycode(struct c8vm_keyboard *c8vm_keyboard, char key)
{
	char lower = tolower(key);

	if (lower >= C8VM_ASCII_SIZE) {
		return C8VM_KEYCODE_INVALID;
	}

	enum c8vm_keycode key_code = c8vm_keyboard->key_map[lower];

	if (key_code == C8VM_KEYCODE_INVALID || key_code >= C8VM_KEYBOARD_KEY_SIZE) {
		return C8VM_KEYCODE_INVALID;
	}

	return key_code;
}

char c8vm_keyboard_keycode_to_key(struct c8vm_keyboard *c8vm_keyboard, enum c8vm_keycode keycode)
{
	if (keycode == C8VM_KEYCODE_INVALID || keycode >= C8VM_KEYBOARD_KEY_SIZE) {
		return -1;
	}

	for (int i = 0; i < C8VM_ASCII_SIZE; i++) {
		if (c8vm_keyboard->key_map[i] == keycode) {
			return (char)i;
		}
	}

	return -1;
}
