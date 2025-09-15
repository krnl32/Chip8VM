#ifndef C8VM_PLATFORM_H
#define C8VM_PLATFORM_H

#include "chip8vm/vm.h"

#include <SDL_render.h>
#include <SDL_video.h>

struct c8vm_platform {
	bool running;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

struct c8vm_platform *c8vm_platform_create();
void c8vm_platform_destroy(struct c8vm_platform *c8vm_platform);

void c8vm_platform_poll(struct c8vm_platform *c8vm_platform, struct c8vm_vm *c8vm_vm);
void c8vm_platform_render(struct c8vm_platform *c8vm_platform, const struct c8vm_vm *c8vm_vm);
void c8vm_platform_delay(struct c8vm_platform *c8vm_platform, uint32_t ms);

char c8vm_platform_wait_for_key_press();

#endif
