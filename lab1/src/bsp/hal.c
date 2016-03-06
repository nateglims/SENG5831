/* Avr Includes */
#include <avr/io.h>
#include <avr/interrupt.h>

/* Local Includes */
#include "VirtualSerial.h"
#include "hal.h"

/* Initialize Hal */
void hal_initialize()
{
  /* Setup LEDs */
  configure_led_ports();

  /* Setup Serial */
  SetupHardware();
  sei();
}

/* Service routine for the hal. Must be called occasionally. */
void hal_service()
{
  USB_Mainloop_Handler();
}
