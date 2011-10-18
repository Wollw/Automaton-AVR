/*	Automaton AVR - automaton.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	The functions used to change the state of the automaton are defined here.
 *
 *	'automaton_init' is used to setup the initial state of the automaton and 
 *	gets passed the size (number of cells) of the automaton.  It gets the 
 *	initial life state of each cell by calling 'initial_state_read' from
 *	'initial_state.h'. It also sets up neighbors.
 *
 *	'automaton_destroy' simply frees the memory that was malloc'ed.
 *
 *	'automaton_update' is the meat of the simulator.  It checks the rules
 *	and applies them to each cell in the automaton.  After getting the next
 *	state of each cell it makes the next state their current state.  It also
 *	outputs their new state over the serial port.
 *
 *	'automaton_delay' is just a call to _delay_ms with the delay specified in
 *	config.h
 *
 *	Format for serial output starts with bit 0 on the far LEFT of the screen.
 *	Example: '011' means cell 0 is off, and cells 1 and 2 are on.
 *
*/
#include <stdlib.h>
#include <util/delay.h>
#include "automaton.h"
#include "rules.h"
#include "initial_state.h"
#include "serial.h"
#include "leds.h"
#include "config.h"

const uint32_t AUTOMATON_NEIGHBORS[] = CONFIG_NEIGHBORS;

/*
 *	Initialize the state of the automaton.  Sets size based on passed value.
 *	Also assigns neighbors based on values in config.h
*/
void automaton_init(automaton_t *automaton) {
	// Set number of cells in automaton
	automaton->size =
		sizeof(AUTOMATON_NEIGHBORS) / sizeof(AUTOMATON_NEIGHBORS[0]);

	// Allocate memory for cells
	automaton->cells = (cell_t *)malloc(automaton->size * sizeof(cell_t));
	if (automaton->cells == NULL)
		return;

	// Set the cells' initial states
	uint32_t initial_state = initial_state_read();
	for (uint8_t i = 0; i < automaton->size; i++) {
		automaton->cells[i].state =
			(initial_state & ((uint32_t)1 << i)) ? ON : OFF ;
		automaton->cells[i].neighbors = AUTOMATON_NEIGHBORS[i];
	}

	// Obtain the rules for the automaton
	rules_read(automaton->rules);

	// Set the automaton functions
	automaton->update = &automaton_update;
	automaton->destroy = &automaton_destroy;
	automaton->delay = &automaton_delay;
	automaton->display = &automaton_display;
}

/*
 * Deallocate the malloc'ed memory and cast cell pointer to NULL.
*/
void automaton_destroy(automaton_t *automaton) {
	free(automaton->cells);
	automaton->cells = NULL;
}

/*
	Update cells based on provided rules. Output over serial.
*/
void automaton_update(automaton_t *automaton) {

	for (uint8_t i = 0; i < automaton->size; i++) {
		uint8_t n = 0;
		for (uint8_t j = 0; j < automaton->size; j++) {
			if ( (automaton->cells[i].neighbors >> j) & 1)
				if (automaton->cells[j].state == ON)
					n++;
		}

		// Survival Rules
		if (automaton->cells[i].state == ON) {
			if ( (automaton->rules->survival >> n) & 1 )
				automaton->cells[i].state_next = ON;
			else
				automaton->cells[i].state_next = OFF;
		}

		// Birth Rules
		else if (automaton->cells[i].state == OFF) {
			if ( (automaton->rules->birth >> n) & 1 )
				automaton->cells[i].state_next = ON;
			else
				automaton->cells[i].state_next = OFF;
		}
	}

	// Update state and calculate number of living cells
	for (uint8_t i = 0; i < automaton->size; i++) {
		automaton->cells[i].state = automaton->cells[i].state_next;
	}
}

// Delay for the set delay time for the automaton
void automaton_delay() {
	_delay_ms(CONFIG_DELAY_MS);
}

// Display the automaton state
void automaton_display(automaton_t *automaton) {
	// Output to LED drivers
	leds_update(automaton);
	
	// Also write to serial port
	for (uint8_t i = 0; i < automaton->size; i++) {
		serial_write('0'+automaton->cells[i].state);
	}
	serial_write('\r');
	serial_write('\n');
}
