#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>

/* Function prototypes */
void init();
void set_debug_led(uint8_t value);
void toggle_debug_led();

/* constants */
uint8_t button_released = 0;

int main()
{
    init();

    while (1)
    {
        if (button_released)
        {
            toggle_debug_led();
            button_released = 0;
        }
        _delay_ms(500);
    }
    return 0;
}


void init()
{
    /* Setup button */
    DDRB &= ~(1 << DDB3); /* Set as input */
    PORTB |= (1 << PORTB3); /* Activate pull up */

    /* Setup "Debug" led */
    DDRB |= (1 << DDB0);

    /* Setup timer3 for 40hz */
    TCCR3A = 0x00;
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    OCR3A = 6250;
    TIMSK3 = (1 << OCIE3A);

    sei();
}


void toggle_debug_led()
{
    PORTB ^= (1 << PORTB0);
}

uint8_t get_button_state()
{
    if (PINB & (1 << PB3))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

ISR(TIMER3_COMPA_vect)
{
    static uint8_t state = 0;
    state = (state << 1) | !get_button_state();
    if (state == 0x80)
        toggle_debug_led();
}
