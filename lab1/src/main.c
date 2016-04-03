/* System Includes */
#define F_CPU 16000000
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

/* App Includes */
#include "types.h"
#include "time.h"
#include "led.h"
#include "htask.h"
#include "menu.h"
#include "button.h"

/* Forward declarations */
void init();

/* Globals */
bool hough_released_flag = false;
bool menu_flag = true;
counts_t counts;

int main()
{
    init();
    /* Run forever */
    while (1)
    {
        if (menu_flag)
        {
          disable_leds();
          handle_prompt(&counts);
          menu_flag = false;
        }

        red_led_task(&counts.red_led);

        if (hough_released_flag)
        {
            hough_transform_task(&counts.hough);
            counts.hough++;
            hough_released_flag = false;
        }
    }
}

void init()
{
    srand(0); /* Obviously not a very good seed but useful for testing... */
    memset(&counts, 0, sizeof(counts));
    time_init();
    led_init();
    menu_init();
    button_init();
    enable_leds();
    sei();
}


/* Timer 0 - Used for tracking global time. */
ISR(TIMER0_COMPA_vect)
{
    time_task();
}

/* Timer 1 COMP B - Used for tracking toggles of the Green LED */
ISR(TIMER1_COMPB_vect)
{
    hough_released_flag = true;
    green_led_task(&counts.green_led);
    if (button_task())
      menu_flag = true;
}

/* Timer 3 - Manages the Yellow LED */
ISR(TIMER3_COMPA_vect)
{
    uint8_t rand_int;
    rand_int = rand();
    yellow_led_task(&counts.yellow_led);
    jitter_led_task(rand_int, &counts.jitter_led);
    if (button_task())
      menu_flag = true;
}
