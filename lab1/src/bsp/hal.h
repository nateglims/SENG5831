#ifndef _HAL_HEADER_GUARD_
#define _HAL_HEADER_GUARD_

/* Public functions */
void hal_initialize();
void hal_service();
void delay_1sec();

/* Sort of public functions... */
uint8_t debounce();

/* Public Types */
typedef enum led_state {
  LED_ON,
  LED_OFF,
} LedState;

typedef enum button_state {
    RELEASED,
    NOT_RELEASED,
} button_state_t;

LedState red_led_state, green_led_state, yellow_led_state, jitter_led_state;
button_state_t button_a_state;

/* Private types */
typedef _raw_button_state_t uint8_t;

/* ADT sort of things. */
struct LED {
    /* Private */
    uint8_t port;
    uint8_t pin;
    uint8_t active_high_flag;
    /* Public */
    void (*set_state)(LedState);
    void (*toggle_state)();
    LedState (*get_state)();
} GreenLED, RedLED, YellowLED, JitterLED;

struct BUTTON {
    /* Private */
    uint8_t port;
    uint8_t pin;
    button_state_t (*_debounce)();
    _raw_button_state_t (*_get_button_state)();
    /* Public */
    button_state_t (*check_debounced_button)();
} ButtonA;

#endif /* _HAL_HEADER_GUARD_ */
