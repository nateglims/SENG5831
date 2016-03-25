#ifndef _led_h
#define _led_h

#include <stdint.h>
#include <avr/io.h>
#include "types.h"

/* Constants */

/* Public Variables */
uint16_t red_task_misses;

/* Public Functions */
void led_init();
void disable_leds();
void enable_leds();
void set_debug_led(bool value);

/* Were this production code, I would warn you that each of these functions are destructive. */
void red_led_task(uint16_t * counter);
void yellow_led_task(uint16_t * counter);
void jitter_led_task(uint8_t rand_int, uint16_t * counter);
void green_led_task(uint16_t * counter);

/* This one isn't destructive, but it fiddles with hardware and expects the user to check the value first */
uint16_t led_set_green_led_period(uint16_t period);

#endif
