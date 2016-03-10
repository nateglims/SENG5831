/* System Includes */
#include <avr/io.h>

/* Local Includes */
#include "hal.h"

uint8_t debounce()
{
    static uint8_t button_shift_register = 0;
    button_shift_register = (button_shift_register << 1) | button.get_state();
    if (button_shift_register == 0b10000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
