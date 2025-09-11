#ifndef C8VM_TIMER_H
#define C8VM_TIMER_H

#include "chip8vm/register.h"

#define C8VM_TIMER_INTERVAL_MS (1000.0 / 60.0)

struct c8vm_timer {
	uint64_t last_tick_ms;
};

struct c8vm_timer *c8vm_timer_create();
void c8vm_timer_destroy(struct c8vm_timer *c8vm_timer);
void c8vm_timer_update(struct c8vm_timer *c8vm_timer, struct c8vm_register *c8vm_register);

#endif
