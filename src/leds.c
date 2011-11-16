/*	Automaton AVR - leds.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	Functions used for handling the output LEDs of the petridish
 *   
 *   'leds_shift_init' just sets up the pins needed to use the shift out
 *	registers that control the LEDs.
 *
 *	'leds_change_state' handles the shifting of the actual states to the
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
	LEDS_DDR |= _BV(LEDS_RCLK) | _BV(LEDS_SRCLK) | _BV(LEDS_SER);
}

/*
	shift a pin state out.  pins_to_shift is how many bits to shift
	buffering with zero instead of the new_state bit values
*/
void leds_change_state(uint32_t new_state, uint8_t pins_to_shift) {
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


