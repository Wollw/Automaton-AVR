/*	Automaton AVR - main.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	This program simulates a Celullar Automaton with an AVR MCU connected to:
 *		(2) 4021 parallel->serial shift registers for reading
 *			survival rules from dip switches
 *		(2) 4021 parallel->serial shift registers for reading
 *			birth rules from dip switches
 *		(3) 4021 used to read initial cell state from
 *		(3) 74HC595 for controlling 24 different LEDs
 *
 *	At start it sets the pins for input and output up.  It then reads the 
 *	rules and initializes the petridish.  While initializing the petridish
 *	the MCU reads the initial state to seed the automaton.  It then enters
 *	the main loop.
 *
 *	The main loop has three steps:
 *		Display - Output a new value to the shift registers handling the LEDs
 *		Delay   - Pause execution for a number of milliseconds to show frame
 *		Update  - Calculate the next states of the cells
 *	
 *	The next state is output over serial at the end of the Update call.  This
 *	occurs right before the new state is displayed so the newest line of
 *	the serial data is the same as the state displayed with the LEDs once
 *	the delay begins.
 *
 *	While it should never be reached (due to the infinite nature of the loop)
 *	all allocated memory is freed after the loop.
*/

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "automaton.h"
#include "initial_state.h"
#include "rules.h"
#include "leds.h"
#include "config.h"

int main(void){
	// Turn the serial port on
	serial_init();

	// Initialize the shift registers connected to
	// the switches used to configure the rules
	// and the initial state.
	initial_state_shift_init();
	rules_shift_init();
	leds_shift_init();

	// Read the configuration information
	// from the switches and initialize the
	// automaton
	rules_t rules;
	rules_read(&rules);

	// Setup the petridish
	petridish_t petridish = {
		0, &rules, 0,
		&automaton_init,
		&automaton_update,
		&automaton_destroy,
		&leds_update,
		&_delay_ms
	};
	petridish.init(&petridish, MY_CELL_COUNT);

	// Start running the simulation
	for (;;) {

		petridish.display(&petridish);
		petridish.delay(MY_DELAY_MS);
		petridish.update(&petridish);

	}

	// Free the memory used by the cells in the petridish.
	// Probably never called but it's here for good form.
	petridish.destroy(&petridish);

	return 0;
}


