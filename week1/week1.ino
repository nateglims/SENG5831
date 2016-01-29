/* Homework 1 */

#include <AStar32U4.h>

AStar32U4ButtonA buttonA;
AStar32U4ButtonC buttonC;

char button;

/* Some of these should static and in a lower scope... */
unsigned long green_last_millis;
unsigned long yellow_last_millis;

enum LED_STATE {
  ON,
  BLINK,
  OFF
} green_state, yellow_state;

/* again should be static... */
bool green_led_state = false;
bool yellow_led_state = false;

void setup()
{
  /* Initialization */
  button = 0;
  green_last_millis = 0;
  yellow_last_millis = 0;
  green_state = OFF;
  yellow_state = OFF;
}

void loop()
{
  button = 0x00;
  if (buttonA.getSingleDebouncedPress()) { button = 'a'; }

  if (buttonC.getSingleDebouncedPress()) { button = 'c'; }

  if (button == 'a') {
    green_advance_state();
  } else if (button == 'c') {
    yellow_advance_state();
  }

  unsigned long current_millis = millis();
  if (current_millis - green_last_millis >= 250) {
    green_state_execute();
    green_last_millis = current_millis;
  }

  if (current_millis - yellow_last_millis >= 1250) {
    yellow_state_execute();
    yellow_last_millis = current_millis;
  }
  delay(50); /* Without this I will crash sometimes*/
  
}

void green_state_execute()
{
  if (green_state == ON)
  {
    ledGreen(1);
  }
  else if (green_state == OFF)
  {
    ledGreen(0);
  }
  else if (green_state == BLINK)
  {
    ledGreen(green_led_state);
    green_led_state = !green_led_state;
  }
}

void yellow_state_execute()
{
  if (yellow_state == ON)
  {
    ledYellow(1);
  }
  else if (yellow_state == OFF)
  {
    ledYellow(0);
  }
  else if (yellow_state == BLINK)
  {
    ledYellow(yellow_led_state);
    yellow_led_state = !yellow_led_state;
  }
}

void yellow_advance_state()
{
  if (yellow_state == ON)
    yellow_state = BLINK;
  else if (yellow_state == BLINK)
    yellow_state = OFF;
  else if (yellow_state = OFF)
    yellow_state = ON;
}

void green_advance_state()
{
  if (green_state == ON)
    green_state = BLINK;
  else if (green_state == BLINK)
    green_state = OFF;
  else if (green_state == OFF)
    green_state = ON;
}

