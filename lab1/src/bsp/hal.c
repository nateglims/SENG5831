/* Avr Includes */
#include <avr/io.h>

/* Util Includes */
#define F_CPU 16000000
#include <util/delay.h>

/* Local Libraries */
#include "hal.h"

/*** LED ***/

/* Configure ports for LEDs*/
void configure_led_ports()
{
  DDRD |= (1 << DDD5) | (1 << DDD6);
  DDRB |= (1 << DDB4) | (1 << DDB6);
}


/* Red LED - Pin B4 */

void set_red_led_state(LedState state)
{
  if (state == LED_ON)
  {
    PORTB |= (1 << PORTB4);
  }
  else if (state == LED_OFF)
  {
    PORTB &= ~(1 << PORTB4);
  }
}

void toggle_red_led()
{
  PORTB ^= (1 << PORTB4);
}

LedState get_red_led_state()
{
  return red_led_state;
}


/* Yellow LED - Pin D6 */

void set_yellow_led_state(LedState state)
{
  if (state == LED_ON)
  {
    PORTD |= (1 << PORTD6);
  }
  else if (state == LED_OFF)
  {
    PORTD &= ~(1 << PORTD6);
  }
}

void toggle_yellow_led()
{
  PORTD ^= (1 << PORTD6);
}

LedState get_yellow_led_state()
{
  return yellow_led_state;
}

/* Green LED - Pin B6 */

void set_green_led_state(LedState state)
{
  if (state == LED_ON)
  {
    PORTB |= (1 << PORTB6);
  }
  else if (state == LED_OFF)
  {
    PORTB &= ~(1 << PORTB6);
  }
}

void toggle_green_led()
{
  PORTB ^= (1 << PORTB6);
}

LedState get_green_led_state()
{
  return green_led_state;
}

/* Jitter LED - Onboard Green */
void set_jitter_led_state(LedState state)
{
  if (state == LED_ON)
  {
    PORTD &= ~(1 << PORTD5);
  }
  else if (state == LED_OFF)
  {
    PORTD |= (1 << PORTD5);
  }
}

void toggle_jitter_led()
{
  PORTD ^= (1 << PORTD5);
}

LedState get_jitter_led_state()
{
  return jitter_led_state;
}

/*** Delay ***/

void delay_1sec()
{
  _delay_ms(1000);
}

/* Initialize Hal
 */
void initialize_hal()
{
  /* Setup LEDs */
  configure_led_ports();

  /* Setup LED Functions */
  RedLED.set_state = set_red_led_state;
  RedLED.toggle_state = toggle_red_led;
  RedLED.get_state = get_red_led_state;

  YellowLED.set_state = set_yellow_led_state;
  YellowLED.toggle_state = toggle_yellow_led;
  YellowLED.get_state = get_yellow_led_state;

  GreenLED.set_state = set_green_led_state;
  GreenLED.toggle_state = toggle_green_led;
  GreenLED.get_state = get_green_led_state;

  JitterLED.set_state = set_jitter_led_state;
  JitterLED.toggle_state = toggle_jitter_led;
  JitterLED.get_state = get_jitter_led_state;

  /* */
}
