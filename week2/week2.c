#include <stdbool.h>
#include <assert.h>
#define F_CPU 16000000
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>

enum fourhzled {
    YELLOW,
    RED,
} led_state;

enum ISRSTATE {
    PRESSED,
    RELEASED,
} button_state;

ISR (PCINT0_vect)
{
    static uint8_t port_b;
    volatile uint8_t i;
    volatile uint32_t j = 0;
    if (button_state == RELEASED) {
        port_b = PINB;
        _delay_ms(50);
        if (port_b == PINB) {
            button_state = PRESSED;
        } else {
            port_b = 0;
        }
    } else if (button_state == PRESSED) {
        if (!(port_b & 0b00001000)) {
            if (led_state == YELLOW) {
                for (i=0; i<8; i++) {
                    PORTC ^= (1 << PORTC7);
                    for (j=0; j<50000; j++);
                    /* _delay_ms(125); */
                }
            }
            else if (led_state == RED) {
                DDRB |= (1 << DDB0);
                for (i=0; i<8; i++) {
                    PORTB ^= (1 << PORTB0);
                    for (j=0; j<50000; j++);
                    /* _delay_ms(125); */
                }
                PORTB |= (1 << PORTB0);
                DDRB &= ~(1 << DDB0);
            }
        } else if (!(port_b & 0b00000001)) {
            if (led_state == YELLOW)
                led_state = RED;
            else if (led_state == RED)
                led_state = YELLOW;
        } else {
            PORTC |= (1 << PORTC7);
            PORTD &= ~(1 << PORTD5);
            PORTB &= ~(1 << PORTB0);
        }
        port_b = 0x00;
        button_state = RELEASED;
    }
}

void init()
{
    /* LED Setup */
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5);
    // DDRB |= (1 << DDB0);
    led_state = YELLOW;
    button_state = RELEASED;

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

