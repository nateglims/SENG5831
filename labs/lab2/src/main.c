/* System Includes */
#define F_CPU 16000000
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

/* App Includes */
#include "motor.h"
#include "VirtualSerial.h"
#include "menu.h"
//#include "interpolator.h"

int16_t torque;
pid_gains_t pid_gains;

void init()
{
  memset(&pid_gains, 0, sizeof(pid_gains));
  SetupHardware();
  motor_init();
  set_setpoint(3000);
  update_gains(1,0,0, &pid_gains);

  menu_init();
  sei();
}

int main()
{
  init();
  while (1)
  {
    USB_Mainloop_Handler();
    printf("Main Loop.\r\n");
    run_menu(&pid_gains);
    _delay_ms(100);
  }
    return 0;
}



/* Timer 0 - Update the torque */
ISR(TIMER0_COMPA_vect)
{
  torque = update_pid(&pid_gains);
}

ISR(PCINT0_vect)
{
  /* We are designating Pin B4 to be Encoder 1 and pin B5 to be encoder 2 */
  static uint8_t previous_encoder1 = 0;
  static uint8_t previous_encoder2 = 0;
  uint8_t current_encoder1, current_encoder2;

  current_encoder1 = (PINB & _BV(PB4)) ? 1 : 0;
  current_encoder2 = (PINB & _BV(PB5)) ? 1 : 0;

/* Debugging stuff...
  printf("C1? %d\tC2? %d\r\n", current_encoder1, current_encoder2);
  printf("P1? %d\tP2? %d\r\n", previous_encoder1, previous_encoder2);
*/

  if (current_encoder2 ^ previous_encoder1)
    update_position(1);
  else if (current_encoder1 ^ previous_encoder2)
    update_position(-1);

  previous_encoder1 = current_encoder1;
  previous_encoder2 = current_encoder2;
}
