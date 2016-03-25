#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

ISR(TIMER1_COMPA_vect)
{
   PORTC ^= (1 << PORTC7); 
}

void init()
{
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);
    TCCR1A = 0x00;
    TCCR1B = (1 << CS10) | (1 << CS12) | (1 << WGM12);
    OCR1A = 15624;
    TCNT1 = 0x0000;
    TIMSK1 = (1 << OCIE1A);
    USBCON = 0x00;
    sei();
}

int main()
{
    init();
    PORTD &= ~(1 << PORTD5);

    PORTC &= ~(1 << PORTC7);
    while (1)
    {
        PORTD ^= (1 << PORTD5);
        _delay_ms(1000);
    }
}

