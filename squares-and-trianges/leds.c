/*
*   Functions used for handling the output LEDs of the automaton
*   
*   led_shift_init
*       setup the pins needed to control the LEDs
*
*   update_leds
*       update the LEDs based on the bits of the number new_states
*
*/
#include <avr/io.h>
#include <util/delay.h>
#include "leds.h"

/*
    Initialize the pins used for the Serial->Parallel shift
    register needed to control LEDs
*/
void led_shift_init(void) {
    // 
    DDRB |= _BV(RCLK) | _BV(SRCLK) | _BV(SER);
}

// shift a number out.  pins_to_shift is how many bits to shift
// buffering with zero instead of the new_states' bit value
void update_leds(uint32_t new_states, uint8_t pins_to_shift) {
    PORTB &= ~_BV(RCLK);

    for (uint8_t i = 0; i < SHIFT_OUT_PIN_COUNT; i++) {
        PORTB &= ~_BV(SRCLK);
        if (pins_to_shift && new_states & ((uint32_t)1 << i)) {
            PORTB |= _BV(SER);
            pins_to_shift--;
        }
        PORTB |= _BV(SRCLK);
        PORTB &= ~_BV(SER);
    }
    PORTB |= _BV(RCLK);

}

