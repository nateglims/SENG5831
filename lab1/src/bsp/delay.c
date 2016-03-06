/* System Util Includes */
#define F_CPU 16000000
#include <util/delay.h>

/* Local Includes */
#include "hal.h"

/*** Delay ***/
void delay_1sec()
{
  _delay_ms(1000);
}
