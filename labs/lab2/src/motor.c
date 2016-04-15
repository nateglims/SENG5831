#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "VirtualSerial.h"

#include "motor.h"
/* Constants, variables, things like that */
static int16_t encoder_count;
static int16_t motor_setpoint;

static int16_t global_torque;
static int16_t global_error;

volatile uint8_t motor_run_flag;

/* Forward declarations for private functions */
void set_forward();
void set_reverse();

/* Public Functions */
void motor_init()
{
  /* Reset local data */
  reset_position();
  set_setpoint(0);

  /* Prep the PWM using Timer 1. This drives motor 2. */
  TCCR1A = _BV(COM1B1) | _BV(WGM11); /* WGM 11, 13 gives a mode of Phase Correct PWM */
  TCCR1B = _BV(CS10) | _BV(WGM13); /* Prescaler 1 to enable */
  ICR1 = 0xFFF;
  OCR1B = 0; /* Start with 0 */
  DDRB |= _BV(DDB6);

  /* Set direction control to output */
  DDRE |= _BV(DDE2);

  /* Enable PCINT 4 and 5 for encoder capture. */
  DDRD &= ~(_BV(DDB4) | _BV(DDB5));
  PCMSK0 = _BV(PCINT5) | _BV(PCINT4);
  PCICR =_BV(PCIE0);


  /* Prep the control loop timer using Timer 0 */
  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01) | _BV(CS00); /* CTC Mode */
  OCR0A = 249; /* Between the prescaler and this, we should be at 1ms */
  TIMSK0 = _BV(OCIE0A);
  TCNT0 = 0;

  motor_run_flag = 0;
}

void run_motor()
{
    motor_run_flag = 1;
}

uint8_t is_motor_running()
{
    return motor_run_flag;
}

/*
 * Encoder count tracking functions
 */
int16_t get_position()
{
  return encoder_count;
}

void update_position(int8_t val)
{
  encoder_count += val;
}

void reset_position()
{
  encoder_count = 0;
}

/* Set the set point in encoder ticks */
void set_setpoint(int16_t setpoint)
{
  motor_setpoint = setpoint;
}

uint16_t get_setpoint()
{
    return motor_setpoint;
}

/* Some debug stuff. */
uint16_t get_torque()
{
    return global_torque;
}

uint16_t get_error()
{
    return global_error;
}


/* This should be called in a timer set for a period of N ms */
int16_t update_pid(pid_gains_t * pid_gains)
{
    if (!motor_run_flag)
        return 0;

  int16_t error;
  int16_t torque;
  static int16_t last_error = 0; /* This might actually lead to a derivative kick */
  error = motor_setpoint - encoder_count;
  torque = pid_gains->kp * error - pid_gains->kd * (error - last_error);
  last_error = error;

/* State management crap */
  if (error == 0)
  {
    motor_run_flag = 0;
    torque = 0;
  }
  global_error = error;
  global_torque = torque;

/* Motor Reversal Stuff */
  if (torque < 0)
  {
    torque = 0 - torque * -1;
    set_reverse();
  }
  else
  {
    set_forward();
  }

 /* Hopefully this takes care of windup. In HVAC controllers
  * at honeywell, we tracked integrator saturation by allowing the control to
  * reach 150% of the setpoint but still clamping the output to 100. This way
  * the controller could notify the Building Automation System of the fault. In
  * our case, since we are basically controlling position right now and not
  * using an integrator, we should eventually be able to reduce our windup.
  */
  if (torque > 4095)
    torque = 4095;

  /* This is where the motor becomes pretty ineffective with our settings */
  if (torque < 110 && motor_run_flag)
    torque = 110;

  OCR1B = torque;

  return error; /* Really for debug purposes */
}
void update_gains(int8_t kp, int8_t kd, int8_t ki, pid_gains_t * gains)
{
  gains->kp = kp;
  gains->kd = kd;
  gains->ki = ki;
}

/* Private functions */
void set_reverse()
{
  PORTE |= _BV(PORTE2);
}

void set_forward()
{
  PORTE &= ~(_BV(PORTE2));
}
