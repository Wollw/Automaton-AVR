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
 *	rules and initializes the automaton.  While initializing the automaton
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
#include "serial.h"
#include "initial_state.h"
#include "rules.h"
#include "leds.h"
#include "automaton.h"

// Setup the IO pins
void setup(void) {
	// Turn the serial port on
	serial_init();

	// Initialize the shift registers connected to
	// the switches used to configure the rules
	// and the initial state.
	initial_state_shift_init();
	rules_shift_init();
	leds_shift_init();
}

int main(void) {
	// Setup pins
	setup();

	// Create the petridish
	automaton_t *petridish = automaton_new();

	// Start the simulation loop
	for (;;) {

		petridish->display(petridish);
		petridish->delay();
		petridish->update(petridish);

	}

	// Free the memory used by the cells in the automaton.
	// Program should never actually get here.
	petridish->destroy(petridish);

	return 0;
}
