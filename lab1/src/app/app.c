#include "hal.h"
int main()
{
  initialize_hal();

  /* Run forever */
  while (1)
  {
    delay_1sec();

    RedLED.toggle_state(LED_ON);
    YellowLED.toggle_state(LED_ON);
    GreenLED.toggle_state(LED_ON);
    JitterLED.toggle_state(LED_ON);
  }
}
