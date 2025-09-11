#include "chip8vm/timer.h"
#include "chip8vm/utility.h"

#include <stdlib.h>
#include <math.h>

struct c8vm_timer *c8vm_timer_create()
{
	struct c8vm_timer *c8vm_timer = malloc(sizeof(struct c8vm_timer));

	if (!c8vm_timer) {
		perror("malloc");
		return NULL;
	}

	c8vm_timer->last_tick_ms = c8vm_utility_get_time_ms();

	return c8vm_timer;
}

void c8vm_timer_destroy(struct c8vm_timer *c8vm_timer)
{
	free(c8vm_timer);
}

void c8vm_timer_update(struct c8vm_timer *c8vm_timer, struct c8vm_register *c8vm_register)
{
	uint64_t now = c8vm_utility_get_time_ms();
	double elapsed = (double)(now - c8vm_timer->last_tick_ms);

	uint8_t ticks = (uint8_t)floor(elapsed / C8VM_TIMER_INTERVAL_MS);

	if (ticks > 0) {
		uint8_t dt = c8vm_register_read_dt(c8vm_register);

		if (dt > 0) {
			uint8_t dt2 = dt > ticks ? dt - ticks : 0;
			c8vm_register_write_dt(c8vm_register, dt2);
		}

		uint8_t st = c8vm_register_read_st(c8vm_register);

		if (st > 0) {
			uint8_t st2 = st > ticks ? st - ticks : 0;
			c8vm_register_write_st(c8vm_register, st2);
		}

		c8vm_timer->last_tick_ms += (uint64_t)(ticks * C8VM_TIMER_INTERVAL_MS);
	}
}
