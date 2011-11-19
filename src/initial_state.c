/*	Automaton AVR - initial_state.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	Functions for providing the initial state of the petridish.
 *	
 *	'initial_state_shift_init' sets up the pins needed to read the shift
 *	registers that the switches used to control the initial state are
 *	attached to.
 *
 *	'initial_state_read' simply reads the switches into a uint32_t variable
 *	where each bit is a cell state. The cell number is the bit number so
 *	bits can be accessed by shifting by the cell's id. (shift 4 times to get
 *	cell 4, shift 0 times to get cell 0, etc)
*/
#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "initial_state.h"

/*
	Setup pins for reading initia state
*/
#ifdef CONFIG_USE_SWITCHES_FOR_SETTINGS
void initial_state_shift_init(void) {

	// Latch and Clock set to output
	DDRC |= _BV(INITIAL_STATE_PS) | _BV(INITIAL_STATE_CP);
	// Data In as input
	DDRC &= ~_BV(INITIAL_STATE_DO);

}
#endif

/*
	Read the initial state from switches into a uint32_t
*/
uint32_t initial_state_read() {

	// Use hard coded initial state if no switches are being used
	#ifdef	CONFIG_USE_SWITCHES_FOR_SETTINGS
	uint32_t initial_state = 0;
	INITIAL_STATE_PORT |= _BV(INITIAL_STATE_PS);
	_delay_us(20);
	INITIAL_STATE_PORT &= ~_BV(INITIAL_STATE_PS);
	for (uint8_t i = 0; i < INITIAL_STATE_SHIFT_REG_COUNT * 8; i++) {
		_delay_us(2);

		if ( INITIAL_STATE_PINS & _BV(INITIAL_STATE_DO) ) {
			initial_state |= (uint32_t)1 << i;
		}

		INITIAL_STATE_PORT &= ~_BV(INITIAL_STATE_CP);
		_delay_us(20);
		INITIAL_STATE_PORT |= _BV(INITIAL_STATE_CP);
	}
	#else
	uint32_t initial_state = CONFIG_INITIAL_STATE;
	#endif

	return initial_state;
	
} 
