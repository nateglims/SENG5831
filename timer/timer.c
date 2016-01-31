#include <avr/io.h>

void init()
{
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);
    TCCR1B = (1 << CS10) | (1 << CS12);
    TCNT1 = 0x00;
}

int main()
{
    init();
    PORTC &= ~(1 << PORTC7);
    PORTD &= ~(1 << PORTD5);
    while (1)
    {
        if (TCNT1 > 15625)
        {
            PORTC ^= (1 << PORTC7);
            PORTD ^= (1 << PORTD5);
            TCNT1 = 0;
        }
    }
}

