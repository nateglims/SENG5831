#include "types.h"
#include "button.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/* Sets up pins for button A */
void button_init()
{
  DDRB  &= ~(1 << DDB3);   /* Set as input */
  PORTB |=  (1 << PORTB3); /* Enable pull up */
}

/* Run this in the 40hz ISR. It uses a common technique for FPGAs and small micros. */
bool button_task()
{
  static uint8_t state = 0;
  uint8_t pin_value = PINB & (1 << PB3);
  state = (state << 1) | !(pin_value); /* Append the opposite of the current button read to the state of the button. 0 if open. 1 if grounded. */
  if (state == 0x80) /* When the button has been closed for at least one measurement, and then open for 7 consecutive measurements, this will be true. */
    return true;     /* This will give us a debounce time of 7*25ms = 175 which is a bit long */
  else
    return false;
}
