#include "chip8vm/display.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct c8vm_display *c8vm_display_create()
{
	struct c8vm_display *c8vm_display = malloc(sizeof(struct c8vm_display));

	if (!c8vm_display) {
		perror("malloc");
		return NULL;
	}

	memset(c8vm_display, 0, sizeof(struct c8vm_display));

	return c8vm_display;
}

void c8vm_display_destroy(struct c8vm_display *c8vm_display)
{
	free(c8vm_display);
}

bool c8vm_display_get_pixel(const struct c8vm_display *c8vm_display, uint8_t x, uint8_t y)
{
	assert((x < C8VM_DISPLAY_WIDTH && y < C8VM_DISPLAY_HEIGHT) && "c8vm_display_get_pixel coordinates out of bounds");
	return c8vm_display->pixels[y][x];
}

void c8vm_display_set_pixel(struct c8vm_display *c8vm_display, uint8_t x, uint8_t y, bool state)
{
	assert((x < C8VM_DISPLAY_WIDTH && y < C8VM_DISPLAY_HEIGHT) && "c8vm_display_set_pixel coordinates out of bounds");
	c8vm_display->pixels[y][x] = state;
}

bool c8vm_display_draw_sprite(struct c8vm_display *c8vm_display, uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t count)
{
	assert((x < C8VM_DISPLAY_WIDTH && y < C8VM_DISPLAY_HEIGHT) && "c8vm_display_draw_sprite coordinates out of bounds");
	assert(count <= C8VM_DISPLAY_HEIGHT && "c8vm_display_draw_sprite sprite count out of bounds");

	bool collision = false;

	for (uint8_t sy = 0; sy < count; sy++) {
		uint8_t spr = sprite[sy];

		for (uint8_t sx = 0; sx < 8; sx++) {
			if (!(spr & (0x80 >> sx))) {
				continue;
			}

			uint8_t wx = (sx + x) % C8VM_DISPLAY_WIDTH;
			uint8_t wy = (sy + y) % C8VM_DISPLAY_HEIGHT;

			bool current_pixel = c8vm_display_get_pixel(c8vm_display, wx, wy);
			collision |= current_pixel;
			c8vm_display_set_pixel(c8vm_display, wx, wy, !current_pixel);
		}
	}

	return collision;
}

void c8vm_display_clear(struct c8vm_display *c8vm_display)
{
	memset(c8vm_display->pixels, 0, sizeof(c8vm_display->pixels));
}
