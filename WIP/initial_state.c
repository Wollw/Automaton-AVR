#include <avr/io.h>
#include <util/delay.h>
#include "initial_state.h"

/*
    Read the initial state from switches
*/
uint32_t initial_state_read() {
    uint32_t initial_state = 0;
    INITIAL_STATE_PORT |= _BV(INITIAL_STATE_PS);
    _delay_us(20);
    INITIAL_STATE_PORT &= ~_BV(INITIAL_STATE_PS);

    int i;
    for (i = 0; i < INITIAL_STATE_SHIFT_REG_COUNT * 8; i++) {
        _delay_us(2);

        if ( INITIAL_STATE_PINS & _BV(INITIAL_STATE_DO) )
            initial_state |= (uint32_t)1 << i;
        _delay_us(20);
        INITIAL_STATE_PORT &= ~_BV(INITIAL_STATE_CP);
        INITIAL_STATE_PORT |= _BV(INITIAL_STATE_CP);
    }

    // temporary, REMOVE when switches are connected to MCU
    initial_state = 0b101010101010101010101010;

    return initial_state;
} 
