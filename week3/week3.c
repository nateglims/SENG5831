#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>
#include <util/atomic.h>

/* Macros */
/* Determined via oscope measurements and trial and error. I suppose I could
 * disassemble and count instructions,
 */
#define LOOP_COUNT_100MS 43478

/* Globals :( */
volatile uint32_t time_ms;

enum flag_state {
  FALSE,
  TRUE,
} release_flag;

/* Interrupts */
/* Timer 1 manages system time. */
ISR(TIMER1_COMPA_vect)
{
    time_ms++;
}

/* Timer 3 manages the Yellow LED */
ISR(TIMER3_COMPA_vect)
{
    PORTC ^= (1 << PORTC7);
    if ((PINB & (1 << PB3)) == 0)
    {
      /* Since Timer1 does not preempt this timer, this makes our time
       * management inaccurate.
       */
      _delay_ms(100);
      release_flag = TRUE;
    }
}

/* Local Functions */
void init()
{
    /* Initialize our system time and release flag. */
    time_ms = 0;
    release_flag = 0;

    /* Initizalize our LED Ports */
    DDRC |= (1 << DDC7);
    DDRD |= (1 << DDD5) | (1 << DDD6);
    DDRB |= (1 << DDB6);

    /* Initialize the LED values. On board Green LED (D5) has inverted logic */
    PORTD |= (1 << PORTD5);
    PORTC &= ~(1 << PORTC7);
    PORTD &= ~(1 << PORTD6);
    PORTB &= ~(1 << PORTB6);

    /* Initialize button A */
    DDRB &= ~(1 << DDB3);
    PORTB |= (1 << PB3);

    /* Timer 1 Initialization
     *
     * The goal is to count milliseconds using Timer 1
     */
    TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12);
    OCR1A = 249;
    TCNT1 = 0x0000;
    TIMSK1 = (1 << OCIE1A);

    /* Timer 3 Initialization
     *
     * The goal is to toggle an LED at 1Hz using Timer 3.
     * While in theory you can drift more with a higher prescaler (or maybe that
     * doens't happen on avr?), I've had better luck getting good timing with
     * the large prescaler.
     */
    TCCR3B = (1 << CS12) | (1 << CS10) | (1 << WGM32);
    OCR3A = 15624;
    TCNT3 = 0x0000;
    TIMSK3 = (1 << OCIE3A);

    /* Disable USB Interrupts to avoid issues with transitioning from program
     * to execution
     */
    USBCON = 0x00;

    /* Enable Interrupts */
    sei();
}

void task_intensetask()
{

    /* This is not the smallest type this can be. But hey, what's two bytes? */
    uint32_t i;

    /* First turn on the "red" LED */
    PORTD |= (1 << PORTD6);

    /* Wait for approx 300 ms. */
    /* TODO: Check if this can be inlined with this compiler. */
    for (i = 0; i < 3*LOOP_COUNT_100MS; i++)
    {
        __asm__ __volatile__("nop");
    }

    /* Turn off "red" then turn on "yellow" */
    PORTD &= ~(1 << PORTD6);
    PORTB |= (1 << PORTB6);

    /* Wait for approx 500 ms */
    for (i = 0; i < 5*LOOP_COUNT_100MS; i++)
    {
        __asm__ __volatile__("nop");
    }

    /* Turn off yellow, turn red back on */
    PORTB &= ~(1 << PORTB6);
    PORTD |= (1 << PORTD6);

    /* Wait for approx 400 ms */
    for (i = 0; i < 4*LOOP_COUNT_100MS; i++)
    {
        __asm__ __volatile__("nop");
    }

    /* Turn off the LEDs (yellow is already off) */
    PORTD &= ~(1 << PORTD6);

}

int main()
{
    uint32_t previous_time = 0;
    uint32_t current_time;
    init();

    while (1)
    {
      /* Toggle the green LED based on the system time. */
//        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            current_time = time_ms;
        }

        if ((current_time - previous_time) > 250) {
            PORTD ^= (1 << PORTD5);
            previous_time = current_time;
        }

        /* This messes with the timing of the green led above. We could
         * interweave the green LED check in the task, but if it were really
         * important we should preempt or break the task down into chunks that
         * take less than 250ms and start them at appropriate intervals.
         */
        if (release_flag) {
          task_intensetask();
          release_flag = FALSE;
        }
    }
}
