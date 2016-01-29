#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

uint8_t yellow_led = PORTC7;
uint8_t green_led = PORTD5;

void init();
void set_green_led(uint8_t value);
void set_yellow_led(uint8_t value);

enum BUTTON {
    BUTTON_C,
    BUTTON_B,
    BUTTON_A,
} button;

int main()
{
    init();
    while(1)
    {
        button = BUTTON_B;
        while ((PINB & 0x09) == 0x09);
        _delay_ms(10);

        while((PINB & 0x08) || (PINB & 0x01))
        {
            if ((PINB & 0x08) == 0)
                button = BUTTON_A;
            else if ((PINB & 0x01) == 0)
                button = BUTTON_C;
            else
                set_green_led(1);
        };
        _delay_ms(10);

        if (button == BUTTON_A) {
            set_yellow_led(1);
            _delay_ms(1000);
            set_yellow_led(0);
        } else if (button == BUTTON_C) {
            set_green_led(1);
            _delay_ms(1000);
            set_green_led(0);
        } else {
            set_yellow_led(1);
        }
    }
}

void init()
{
    /* Set the Green and Yellow LED Pins to outputs */
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);

    set_yellow_led(0);
    set_green_led(0);
    /* Button A is PB3, Button C is PB0 */
    /* A: PCINT3, C: PCINT0 */
    DDRB &= ~(1 << DDB3);
    DDRB &= ~(1 << DDB0);

    PORTB |= (1 << PB3);
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

