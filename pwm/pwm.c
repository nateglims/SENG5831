#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

void init()
{
    /* Setup some ports */
    DDRD |= (1 << DDD5);
    DDRB |= (1 << DDB5);

    /* Setup some timers */
    TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);
    TCCR1B = (1 << CS10) | (1 << WGM12) ;

    OCR1A = 0xff;
    /* Disable USB interrupts and enable global */
    USBCON = 0x00;
}

int main()
{
    init();

    while (1)
    {
        _delay_ms(100);
        OCR1A = 0xff;;
        _delay_ms(100);
    }
}
