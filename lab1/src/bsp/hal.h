/* Public functions */
void hal_initialize();
void hal_service();
void delay_1sec();

/* Public Types */
typedef enum led_state {
  LED_ON,
  LED_OFF,
} LedState;

LedState red_led_state, green_led_state, yellow_led_state, jitter_led_state;

/* Function Prototyes */
struct LED_OFF {
  void (*set_state)(LedState);
  void (*toggle_state)();
  LedState (*get_state)();
} GreenLED, RedLED, YellowLED, JitterLED;
