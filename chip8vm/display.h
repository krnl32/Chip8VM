#ifndef C8VM_DISPLAY_H
#define C8VM_DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

/*
	Chip8 Display Specifications
	Display Size: (64x32)
	Display Type: Monochrome
*/
#define C8VM_DISPLAY_WIDTH 64
#define C8VM_DISPLAY_HEIGHT 32
#define C8VM_DISPLAY_SCALER 10

struct c8vm_display {
	bool pixels[C8VM_DISPLAY_HEIGHT][C8VM_DISPLAY_WIDTH];
};

struct c8vm_display *c8vm_display_create(void);
void c8vm_display_destroy(struct c8vm_display *c8vm_display);

bool c8vm_display_get_pixel(const struct c8vm_display *c8vm_display, uint8_t x, uint8_t y);
void c8vm_display_set_pixel(struct c8vm_display *c8vm_display, uint8_t x, uint8_t y, bool state);

int c8vm_display_draw_sprite(struct c8vm_display *c8vm_display, uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t count); /* return 1 if any pixel is erased, 0 if no pixel is erased, -1 for errors */

void c8vm_display_clear(struct c8vm_display *c8vm_display);

#endif
