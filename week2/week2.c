#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>

#define F_CPU 16000000
#include <util/delay.h>

enum fourhzled {
    YELLOW,
    RED,
} led_state;

enum ISRSTATE {
    PRESS,
    RELEASE,
} isr_state;

ISR (PCINT0_vect)
{
    static uint8_t portb;
    uint8_t i;
    uint16_t j = 0;
    _delay_ms(100);
    if (isr_state == PRESS) {
        portb = PINB;
        if (portb) {
            isr_state = RELEASE;
        }
    } else if (isr_state == RELEASE) {
        if (!(portb & 0b00001000)) {
            if (led_state == YELLOW) {
                for (i=0; i<8; i++) {
                    PORTC ^= (1 << PORTC7);
                    for (j=0; j<200; j++);
                    // _delay_ms(125);
                }
            }
            else if (led_state == RED) {
                DDRB |= (1 << DDB0);
                for (i=0; i<8; i++) {
                    PORTB ^= (1 << PORTB0);
                    for (j=0; j< 200; j++);
                    // _delay_ms(125);
                }
                DDRB &= ~(1 << DDB0);
            }
        } else if (!(portb & 0b00000001)) {
            if (led_state == YELLOW)
                led_state = RED;
            else if (led_state == RED)
                led_state = YELLOW;
        } else {
            /* Flash slowly to know when I've hit code I shouldn't */
            for (i=0; i<4; i++) {
                PORTC ^= (1<< PORTC7);
                _delay_ms(1000);
            }
        }
        portb = 0x00;
        isr_state = PRESS;
    }
}

void init()
{
    /* LED Setup */
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);
    // DDRB |= (1 << DDB0);
    led_state = YELLOW;
    isr_state = PRESS;

    /* Setup Button A Pin */
    PORTB |= (1 << PB0) | (1 << PB3);
    DDRB &= ~((1 << DDB3) | (1 << DDB0));

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
    PORTB |= (1 << PORTB0);
    while (1)
    {
        PORTD ^= (1 << PORTD5);
 //        PORTB ^= (1 << PORTB0);
        _delay_ms(1000);
    }
}

