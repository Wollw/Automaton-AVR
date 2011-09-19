// Number of pins avaiable through serial->parallel
// shift registers (74HC595)
#define SHIFT_OUT_PIN_COUNT 24

// Serial->Parallel for LEDs
#define RCLK    PB0
#define SER     PB1
#define SRCLK   PB2

// Functions for interacting with the output for leds
// See leds.c for details
void led_shift_init(void);
void update_leds(uint32_t new_states, uint8_t pins_to_shift);
