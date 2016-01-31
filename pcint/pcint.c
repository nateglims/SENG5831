#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

ISR (PCINT0_vect)
{
    PORTC ^= (1 << PORTC7);
}

void init()
{
    /* LED Setup */
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);

    /* Setup Button Pins */
    PORTB |= (1 << PB0) | (1 << PB3);
    DDRB &= ~((1 << DDB0) | (1 << DDB3));

    /* Interrupt Setup */
    PCICR = 0x01;
    PCMSK0 = (1 << PCINT3) | (1 << PCINT0);
    sei();
}

int main()
{
    init();
    PORTC &= ~(1 << PORTC7);
    PORTD &= ~(1 << PORTD5);
    while (1)
    {
        PORTD ^= (1 << PORTD5);
        _delay_ms(500);
    }
}

