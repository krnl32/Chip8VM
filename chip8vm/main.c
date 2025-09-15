#include "chip8vm/platform.h"
#include "chip8vm/vm.h"
#include "chip8vm/utility.h"
#include "chip8vm/sound.h"

#include <SDL.h>
#include <SDL_keycode.h>
#include <SDL_main.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <stdio.h>
#include <unistd.h>
#include <time.h>

static struct c8vm_platform *c8vm_platform = NULL;
static struct c8vm_vm *c8vm_vm = NULL;

static bool c8vm_init(const char *rompath);
static void c8vm_exit();

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	if (argc <= 1) {
		fprintf(stdout, "usage: %s [ROM PATH]\n", argv[0]);
		return 0;
	}

	srand((unsigned int)time(NULL));
	const char *rompath = argv[1];

	if (access(rompath, R_OK) == -1) {
		perror(rompath);
		return -1;
	}

	if (!c8vm_init(rompath)) {
		printf("c8vm_init failed\n");
		return -1;
	}

	// main loop
	bool sound_playing = false;
	uint64_t last_time = c8vm_utility_get_time_ms();

	while (c8vm_platform->running) {
		uint64_t current_time = c8vm_utility_get_time_ms();

		if (current_time - last_time >= C8VM_TIMER_INTERVAL_MS) {
			// handle delay timer
			c8vm_timer_update(c8vm_vm->c8vm_timer, c8vm_vm->c8vm_register);
			last_time = current_time;

			// handle sound time
			uint8_t st = c8vm_register_read_st(c8vm_vm->c8vm_register);
			bool should_play = (st > 0);

			if (should_play && !sound_playing) {
				c8vm_sound_start();
				sound_playing = true;
			} else if (!should_play && sound_playing) {
				c8vm_sound_stop();
				sound_playing = false;
			}
		}

		for (int i = 0; i < C8VM_CPU_CYCLES_PER_FRAME; i++) {
			c8vm_vm_cpu_cycle(c8vm_vm);
		}

		c8vm_platform_poll(c8vm_platform, c8vm_vm);
		c8vm_platform_render(c8vm_platform, c8vm_vm);
		c8vm_platform_delay(c8vm_platform, 1);
	}

	c8vm_exit();

	return 0;
}

static bool c8vm_init(const char *rompath)
{
	// Setup Platform
	c8vm_platform = c8vm_platform_create();

	if (!c8vm_platform) {
		printf("c8vm_platform_create failed\n");
		return false;
	}

	// Setup VM
	c8vm_vm = c8vm_vm_create();

	if (!c8vm_vm) {
		printf("c8vm_vm_create failed\n");
		return false;
	}

	// Setup Platform Audio
	if (!c8vm_sound_create()) {
		printf("c8vm_sound_create failed\n");
		return false;
	}

	// Remap Keyboard
	enum c8vm_keycode key_codes[] = {
		C8VM_KEY_0, C8VM_KEY_1, C8VM_KEY_2, C8VM_KEY_3, C8VM_KEY_4, C8VM_KEY_5, C8VM_KEY_6, C8VM_KEY_7, C8VM_KEY_8, C8VM_KEY_9, C8VM_KEY_A, C8VM_KEY_B, C8VM_KEY_C, C8VM_KEY_D, C8VM_KEY_E, C8VM_KEY_F,
	};

	char keys[] = {
		SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f,
	};

	c8vm_keyboard_remap(c8vm_vm->c8vm_keyboard, key_codes, keys, C8VM_KEYBOARD_KEY_SIZE);

	// Load Rom
	if (!c8vm_vm_load_rom(c8vm_vm, rompath)) {
		printf("c8vm_vm_load_rom failed\n");
		return false;
	}

	return true;
}

static void c8vm_exit()
{
	c8vm_sound_destroy();
	c8vm_vm_destroy(c8vm_vm);
	c8vm_platform_destroy(c8vm_platform);
}
