/* System Includes */
#include <avr/io.h>

/* Local Includes */
#include "hal.h"

button_state_t button_a_debounce()
{
    static uint8_t button_shift_register = 0;
    button_shift_register = (button_shift_register << 1) | ButtonA.get_state();
    if (button_shift_register == 0b10000000)
    {
        return RELEASED;
    }
    else
    {
        return NOT_RELEASED;
    }
}

_raw_button_state_t _get_button_a_state()
{
  return
}

button_state_t check_button_a_debounce()
{
  return
}

void configure_button()
{
  /* Set Button ports to input */

  /* Set private members */
  ButtonA.port = PORTB;
  ButtonA.pin = PB3;
  ButtonA._debounce = button_a_debounce;
  ButtonA._get_button_state = _get_button_state;

  /* Public Members */
  ButtonA.check_debounced_button = check_button_a_debounce;

}
