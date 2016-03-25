#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

const uint16_t ocrscaler = 0x3ff;
enum STATE { UP, DOWN } led_state;

#define ASM_ROL(reg) __asm__ __volatile__("rol %0" : "=r" (reg) : "0" (reg)) 

void init()
{
    /* Setup some ports */
    DDRD |= (1 << DDD5);
    DDRB |= (1 << DDB5);

    /* Setup some timers */
    TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);
    TCCR1B = (1 << CS10) | (1 << WGM12) ;

    OCR1A = ocrscaler;
    led_state = DOWN;
    /* Disable USB interrupts and enable global */
    USBCON = 0x00;
}

int main()
{
    init();

    while (1)
    {
        OCR1A >>= 1;
        if (OCR1A == 0)
            OCR1A = ocrscaler;
        _delay_ms(100);
    }
}

