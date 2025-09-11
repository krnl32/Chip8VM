#include "chip8vm/vm.h"
#include "chip8vm/utility.h"
#include "chip8vm/sound.h"

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_main.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_timer.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	struct c8vm_vm *c8vm_vm = c8vm_vm_create();

	if (!c8vm_vm) {
		printf("c8vm_vm_create failed\n");
		return -1;
	}

	// Remap Keys
	enum c8vm_keycode key_codes[] = {
		C8VM_KEY_1, C8VM_KEY_2, C8VM_KEY_3, C8VM_KEY_C, C8VM_KEY_4, C8VM_KEY_5, C8VM_KEY_6, C8VM_KEY_D, C8VM_KEY_7, C8VM_KEY_8, C8VM_KEY_9, C8VM_KEY_E, C8VM_KEY_A, C8VM_KEY_0, C8VM_KEY_B, C8VM_KEY_F,
	};

	char keys[] = {
		SDLK_1, SDLK_2, SDLK_3, SDLK_c, SDLK_4, SDLK_5, SDLK_6, SDLK_d, SDLK_7, SDLK_8, SDLK_9, SDLK_e, SDLK_a, SDLK_0, SDLK_b, SDLK_f,
	};

	c8vm_keyboard_remap(c8vm_vm->c8vm_keyboard, key_codes, keys, C8VM_KEYBOARD_KEY_SIZE);

	// Sample
	// c8vm_display_set_pixel(c8vm_vm->c8vm_display, 0, 0, true);
	c8vm_display_draw_sprite(c8vm_vm->c8vm_display, 0, 0, &c8vm_vm->c8vm_memory->memory[0x5], 5);
	c8vm_register_write_dt(c8vm_vm->c8vm_register, 255);
	c8vm_register_write_st(c8vm_vm->c8vm_register, 120); // Set sound timer (st) to 120 (~2 seconds at 60Hz)

	// Setup SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Chip8VM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	if (!window) {
		printf("SDL_CreateWindow failed\n");
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_TEXTUREACCESS_TARGET);

	if (!renderer) {
		printf("SDL_CreateRenderer failed\n");
		return -1;
	}

	if (!c8vm_sound_create()) {
		printf("c8vm_sound_create failed\n");
		return -1;
	}

	// main loop
	bool sound_playing = false;
	uint64_t last_time = c8vm_utility_get_time_ms();

	while (1) {
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

		// handle events
		SDL_Event ev;

		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				goto end;

			case SDL_KEYDOWN:
				c8vm_keyboard_set_key_state(c8vm_vm->c8vm_keyboard, ev.key.keysym.sym, true);
				printf("Key: %c Pressed: %d\n", ev.key.keysym.sym, c8vm_keyboard_is_key_down(c8vm_vm->c8vm_keyboard, ev.key.keysym.sym));
				break;

			case SDL_KEYUP:
				c8vm_keyboard_set_key_state(c8vm_vm->c8vm_keyboard, ev.key.keysym.sym, false);
				printf("Key: %c Released: %d\n", ev.key.keysym.sym, c8vm_keyboard_is_key_up(c8vm_vm->c8vm_keyboard, ev.key.keysym.sym));
				break;
			}
		}

		// clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (uint8_t x = 0; x < C8VM_DISPLAY_WIDTH; x++) {
			for (uint8_t y = 0; y < C8VM_DISPLAY_HEIGHT; y++) {
				bool pixel_set = c8vm_display_get_pixel(c8vm_vm->c8vm_display, x, y);

				if (pixel_set) {
					SDL_Rect rect = { x * C8VM_DISPLAY_SCALER, y * C8VM_DISPLAY_SCALER, C8VM_DISPLAY_SCALER, C8VM_DISPLAY_SCALER };
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}

		SDL_RenderPresent(renderer);
	}

end:
	c8vm_sound_destroy();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	c8vm_vm_destroy(c8vm_vm);

	return 0;
}
