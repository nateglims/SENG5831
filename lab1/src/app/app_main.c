#include <stdio.h>

#include "hal.h"
int main()
{
  static uint16_t value = 1000;
  hal_initialize();

  /* Run forever */
  while (1)
  {
    hal_service();
    delay_1sec();
    printf("[%p] %x\r\n", &value, value--);
    RedLED.toggle_state(LED_ON);
    YellowLED.toggle_state(LED_ON);
    GreenLED.toggle_state(LED_ON);
    JitterLED.toggle_state(LED_ON);
  }
}
