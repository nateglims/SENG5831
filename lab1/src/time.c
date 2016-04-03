#include "time.h"
#include <avr/interrupt.h>

/* This file looks a little thin, but I like the abstraction. */

static uint32_t global_time_ms;


void time_init()
{
    global_time_ms = 0;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);
    TCNT0 = 0;
}


void time_task()
{
    global_time_ms++;
}


uint32_t get_time()
{
    uint32_t the_time;
    cli();
    the_time = global_time_ms;
    sei();
    return the_time;
}
