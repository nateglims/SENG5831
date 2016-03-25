#ifndef _menu_h
#define _menu_h

#include <stdint.h>
#include "types.h"

typedef struct {
    uint8_t experiment_number;
} menu_t;

typedef struct {
    uint16_t hough;
    uint16_t red_led;
    uint16_t green_led;
    uint16_t yellow_led;
    uint16_t jitter_led;
} counts_t;

/* Public Functions */
void menu_init();
menu_t menu();
void report(counts_t * counts);
void handle_prompt();

#endif
