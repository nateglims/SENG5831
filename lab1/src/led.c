#include "led.h"
#include "time.h"
#include "VirtualSerial.h"
#include <stdio.h>
#include "globals.h"

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

/* Globals and stuff */
bool led_enabled_flag;
const uint16_t timer3_40hz = 6250;
volatile uint16_t green_led_counts = 12500;

/**
 * Initializes all the ports
 */
void led_init()
{
    /* Setup ports for all the LEDs */
    DDRD |= (1 << DDD5) | (1 << DDD6);  /* D6 - Yellow, D5 - Jitter */
    DDRB |= (1 << DDB4) | (1 << DDB6);  /* B4 - Red, B6 - Green */

    /* Yellow LED Task Setup - Using Timer 3
     * We are using a prescaler of 64 and CTC to get us a 40hz ISR.
     */
    TCCR3A = 0x00;
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    OCR3A = timer3_40hz;
    TIMSK3 = (1 << OCIE3A);

    /* Green LED Task Setup - Using Timer 1 and PWM
     *
     * This should amount to a prescaler of 256 (CS12), a mode of FAST PWM with
     * the TOP being OCR1A. OCR1B tying the output pin (OC1B) to half of OCR1A
     * to give a duty cycle of 50%, and enabling the interrupt so we can count
     * stuff for the task.
     */
    TCCR1A = (1 << COM1B1) | (1 << COM1B0) | (1 << WGM11)  | (1 << WGM10);
    TCCR1B = (1 << CS12) | (1 << WGM13) | (1 << WGM12);

    /* On Board RED LED is used for some debugging */
    DDRB |= (1 << DDB0);
}

/* Disable LEDs and their counters. */
void disable_leds()
{
  led_enabled_flag = false;
  /* Special stuff to disable green LED */
  OCR1A = 0; /* Disable the green LED itself by setting PWM top to 0 */
  OCR1B = 0;
  /* Also turn them off... */
  PORTB &= ~(1 << PORTB4); /* Red */
  PORTD &= ~(1 << PORTD6); /* Yellow */
  /* Jitter turns off on it's own */
}

/* Enable the LEDs and their counters. */
void enable_leds()
{
  led_enabled_flag = true;
  OCR1A = green_led_counts;
  OCR1B = green_led_counts >> 1;
  TCNT1 = 0;
  TIMSK1 = (1 << OCIE1B);

}

/**
 * Red LED Task - You may be wondering why all these functions have output parameters for an integer.
 */
void red_led_task(uint16_t * counter)
{
    if (!led_enabled_flag)
      return;

    static uint32_t previous_time = 0;
    uint32_t current_time; /* Some day we will be able to intialize to non-constants in C. I expect it in C19 */
    uint32_t difference;

    current_time = get_time();
    difference = current_time - previous_time;

    if (difference == 100)
    {
        previous_time = current_time;
        PORTB ^= (1 << PORTB4);
        *counter = *counter+1;
    }
    else if (difference > 100)
    {
        previous_time = current_time;
        red_task_misses++;
    }
}

/**
 * Yellow LED Task - This is to be run inside a 40Hz timer.
 */
void yellow_led_task(uint16_t * counter)
{
    if (!led_enabled_flag)
      return;

    if (global_experiment == 8) /* This kills the process */
      sei();

    static uint8_t count = 0;
    if (count == 3) /* Every 4th task call. This down samples us from 40 to 10hz */
    {
        PORTD ^= (1 << PORTD6);
        *counter = *counter+1;
        count = 0;
    }
    else
    {
        count++;
    }
    /* Experiment delays */
    if (global_experiment == 3)
      _delay_ms(20);
    else if (global_experiment == 5)
      _delay_ms(30);
    else if (global_experiment == 7 || global_experiment == 8)
      _delay_ms(105);

}


/**
 * Jitter Task - This is to be run inside a 40Hz timer
 */
void jitter_led_task(uint8_t rand_int, uint16_t * counter)
{

    if (!led_enabled_flag)
      return;
    /* Check if the random int is a multiple of 4. Since modulo is expensive we
     * are going to do bit math with magic numbers.
     */
    if (rand_int == (rand_int && 0xFC))
    {
        PORTD &= ~(1 << PORTD5);
        _delay_ms(5); /* Per the spec */
        PORTD |= (1 << PORTD5);
        *counter = *counter+1;
    }
}


/**
 * Counting task for the green pwm led.
 */
void green_led_task(uint16_t * counter)
{
    if (!led_enabled_flag)
      return;
    *counter = *counter+2; /* We toggle twice per interrupt. */
    if (global_experiment == 2)
    {
      set_debug_led(true);
      _delay_ms(20);
    }
    else if (global_experiment == 4)
      _delay_ms(30);
    else if (global_experiment == 6)
      _delay_ms(105);
}

/*
 * Set the period of the green LED in ms. The return value is for debugging.
 */
uint16_t led_set_green_led_period(uint16_t period)
{
    /* Using a prescaler of 256 with a fast PWM gives us a range of 1/(16Mhz/256)
     * to 65535/(16Mhz/256) which equates to 16uSec to 1048ms. Since I really
     * don't want to do floating point operations we are just going to go ahead
     * and say 1 to 1048ms.
     *
     * There are 16000000/256 = 62500 F_timer ticks per second.
     * There are 62.5 ticks per ms and I don't like fractional values so I'm going to cheat...
     */
    const uint16_t ticks_per_ms = 125;  /* Notice how this is twice the value we talked about... */
    uint16_t ocra_value = ticks_per_ms * (period >> 2); /* Ticks * 2 * (period/2) where period is the toggle period (ocra_period / 2). */
    green_led_counts = ocra_value;
    return ocra_value;
}

/* Used for debugging stuff. */
void set_debug_led(bool value)
{
    if (value)
      PORTB &= ~(1 << PORTB0); /* Inverted logic - drive it low to turn on */
    else
      PORTB |= (1 << PORTB0);
}
