#define F_CPU 16000000
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

uint8_t yellow_led = PORTC7;
uint8_t green_led = PORTD5;

void init();
void set_green_led(uint8_t value);
void set_yellow_led(uint8_t value);

int main()
{
    init();
    while(1)
    {
        set_yellow_led(1);
        set_green_led(1);
        _delay_ms(5000);
        set_yellow_led(0);
        set_green_led(0);
        _delay_ms(5000);
    }
}

void init()
{
    /* Set the Green and Yellow LED Pins to outputs */
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);
}

void set_yellow_led(uint8_t value)
{
    /* The pin for the yellow LED needs to be driven high to turn it on */
    if (value == 1)
        PORTC |= (1 << yellow_led);
    else if (value == 0)
        PORTC &= ~(1 << yellow_led); 
}

void set_green_led(uint8_t value)
{
    /* The pin for the green LED needs to be driven low to turn it on */
    if (value == 1)
        PORTD &= ~(1 << green_led);
    else if (value == 0)
        PORTD |= (1 << green_led);
}
