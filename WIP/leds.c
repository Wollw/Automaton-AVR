/*
*   Functions used for handling the output LEDs of the automaton
*   
*   leds_shift_init
*       setup the pins needed to control the LEDs
*
*   leds_update
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
void leds_shift_init(void) {
    // 
    DDRB |= _BV(RCLK) | _BV(SRCLK) | _BV(SER);
}

/*
    Converts the cell states from the petridish
    into individual bits to shift out with leds_shift_out
*/
void leds_update(petridish_t *petridish) {
    uint32_t state = 0;
    int i;
    for (i = 0; i < petridish->size; i++) {
        if ( petridish->cells[i].state == ON )
            state |= (uint32_t)1 << i;
        else
            state &= ~((uint32_t)1 << i);
    }
    leds_shift_out(state, petridish->size);
}

// shift a number out.  pins_to_shift is how many bits to shift
// buffering with zero instead of the new_state bit values
void leds_shift_out(uint32_t new_state, uint8_t pins_to_shift) {
    PORTB &= ~_BV(RCLK);

    for (uint8_t i = 0; i < SHIFT_OUT_PIN_COUNT; i++) {
        PORTB &= ~_BV(SRCLK);
        if (pins_to_shift && new_state & ((uint32_t)1 << i)) {
            PORTB |= _BV(SER);
            pins_to_shift--;
        }
        PORTB |= _BV(SRCLK);
        PORTB &= ~_BV(SER);
    }
    PORTB |= _BV(RCLK);

}

