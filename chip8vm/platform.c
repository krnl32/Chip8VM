#include "chip8vm/platform.h"

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <stdlib.h>
#include <stdio.h>

struct c8vm_platform *c8vm_platform_create()
{
	struct c8vm_platform *c8vm_platform = malloc(sizeof(struct c8vm_platform));

	if (!c8vm_platform) {
		perror("malloc");
		return NULL;
	}

	SDL_Init(SDL_INIT_EVERYTHING);

	c8vm_platform->window = SDL_CreateWindow("Chip8VM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);

	if (!c8vm_platform->window) {
		printf("SDL_CreateWindow failed\n");
		return NULL;
	}

	c8vm_platform->renderer = SDL_CreateRenderer(c8vm_platform->window, 0, SDL_TEXTUREACCESS_TARGET);

	if (!c8vm_platform->renderer) {
		printf("SDL_CreateRenderer failed\n");
		return NULL;
	}

	c8vm_platform->running = true;

	return c8vm_platform;
}

void c8vm_platform_destroy(struct c8vm_platform *c8vm_platform)
{
	if (c8vm_platform) {
		SDL_DestroyRenderer(c8vm_platform->renderer);
		SDL_DestroyWindow(c8vm_platform->window);
		SDL_Quit();

		free(c8vm_platform);
	}
}

void c8vm_platform_poll(struct c8vm_platform *c8vm_platform, struct c8vm_vm *c8vm_vm)
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT:
				c8vm_platform->running = false;
				break;

			case SDL_KEYDOWN:
				c8vm_keyboard_set_key_state(c8vm_vm->c8vm_keyboard, ev.key.keysym.sym, true);
				break;

			case SDL_KEYUP:
				c8vm_keyboard_set_key_state(c8vm_vm->c8vm_keyboard, ev.key.keysym.sym, false);
				break;
		}
	}
}

void c8vm_platform_render(struct c8vm_platform *c8vm_platform, const struct c8vm_vm *c8vm_vm)
{
	// clear screen
	SDL_SetRenderDrawColor(c8vm_platform->renderer, 0, 0, 0, 0);
	SDL_RenderClear(c8vm_platform->renderer);

	SDL_SetRenderDrawColor(c8vm_platform->renderer, 255, 255, 255, 255);

	for (uint8_t x = 0; x < C8VM_DISPLAY_WIDTH; x++) {
		for (uint8_t y = 0; y < C8VM_DISPLAY_HEIGHT; y++) {
			bool pixel_set = c8vm_display_get_pixel(c8vm_vm->c8vm_display, x, y);

			if (pixel_set) {
				SDL_Rect rect = { x * C8VM_DISPLAY_SCALER, y * C8VM_DISPLAY_SCALER, C8VM_DISPLAY_SCALER, C8VM_DISPLAY_SCALER };
				SDL_RenderFillRect(c8vm_platform->renderer, &rect);
			}
		}
	}

	SDL_RenderPresent(c8vm_platform->renderer);
}

void c8vm_platform_delay(struct c8vm_platform *c8vm_platform, uint32_t ms)
{
	SDL_Delay(ms);
}

char c8vm_platform_wait_for_key_press()
{
	SDL_Event ev;

	while (SDL_WaitEvent(&ev)) {
		if (ev.type == SDL_KEYDOWN) {
			return ev.key.keysym.sym;
		}
	}

	return -1;
}
