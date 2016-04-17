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
  update_gains(50, 20, 0, &pid_gains);

  menu_init();
  sei();
}

int main()
{
  init();
  while (1)
  {
      /* For our interpolator we're just going to manually do this */
    printf("Main Loop!\r\n");
    /* First Run */
    printf("90 Degrees forward");
    set_setpoint(550);
    run_motor();
    while(is_motor_running())
        print_motor_stats_for_interpolator();
    _delay_ms(500);
    /* Run again */
    printf("360 Degrees back.\r\n");
    reset_position();
    set_setpoint(-2200);
    run_motor();
    while(is_motor_running())
        print_motor_stats_for_interpolator();
    _delay_ms(500);
    printf("5 degrees forward.\r\n");
    reset_position();
    set_setpoint(31);
    run_motor();
    while(is_motor_running())
        print_motor_stats_for_interpolator();
    /* Delay and repeat to capture the info. */
    _delay_ms(10000);
  }
    return 0;
}



/* Timer 0 - Update the torque */
ISR(TIMER0_COMPA_vect)
{
    static uint8_t counter = 0;
    if (counter == 31)
    {
        torque = update_pid(&pid_gains);
        counter = 0;
    }
    counter++;
}

ISR(PCINT0_vect)
{
  /* We are designating Pin B4 to be Encoder 1 and pin B5 to be encoder 2 */
  static uint8_t previous_encoder1 = 0;
  static uint8_t previous_encoder2 = 0;
  uint8_t current_encoder1, current_encoder2;

  current_encoder1 = (PINB & _BV(PB4)) ? 1 : 0;
  current_encoder2 = (PINB & _BV(PB5)) ? 1 : 0;

  if (current_encoder2 ^ previous_encoder1)
    update_position(1);
  else if (current_encoder1 ^ previous_encoder2)
    update_position(-1);

  previous_encoder1 = current_encoder1;
  previous_encoder2 = current_encoder2;
}
