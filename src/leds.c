/*	Automaton AVR - leds.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	Functions used for handling the output LEDs of the automaton
 *   
 *   'leds_shift_init' just sets up the pins needed to use the shift out
 *	registers that control the LEDs.
 *
 *	'leds_update' gets the state of each cell and packs it into a uint32_t.
 *	it then calls 'leds_shift_out' to do the actual output to the shift
 *	registers.
 *
 *	'leds_shift_out' just handles the shifting of the actual states to the
 *	shift out registers.
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
	// Make the shift register pins outputs
	LEDS_DDR |= _BV(LEDS_RCLK) | _BV(LEDS_SRCLK) | _BV(LEDS_SER);
}

/*
	Converts the cell states from the petridish
	into individual bits to shift out with leds_shift_out
*/
void leds_update(petridish_t *petridish) {
	uint32_t state = 0;

	for (uint8_t i = 0; i < petridish->size; i++) {
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
	LEDS_PORT &= ~_BV(LEDS_RCLK);

	for (uint8_t i = 0; i < LEDS_SHIFT_REG_COUNT * 8; i++) {
		LEDS_PORT &= ~_BV(LEDS_SRCLK);
		if (pins_to_shift && new_state & ((uint32_t)1 << i)) {
			LEDS_PORT |= _BV(LEDS_SER);
			pins_to_shift--;
		}
		LEDS_PORT |= _BV(LEDS_SRCLK);
		LEDS_PORT &= ~_BV(LEDS_SER);
	}

	LEDS_PORT |= _BV(LEDS_RCLK);
}

