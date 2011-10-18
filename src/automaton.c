/*	Automaton AVR - automaton.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	The functions used to change the state of the automaton are defined here.
 *
 *	'automaton_init' is used to setup the initial state of the petridish and 
 *	gets passed the size (number of cells) of the automaton.  It gets the 
 *	initial life state of each cell by calling 'initial_state_read' from
 *	'initial_state.h'. It also sets up neighbors.
 *
 *	'automaton_destroy' simply frees the memory that was malloc'ed.
 *
 *	'automaton_update' is the meat of the simulator.  It checks the rules
 *	and applies them to each cell in the petridish.  After getting the next
 *	state of each cell it makes the next state their current state.  It also
 *	outputs their new state over the serial port.
 *
 *	Format for serial output starts with bit 0 on the far LEFT of the screen.
 *	Example: '011' means cell 0 is off, and cells 1 and 2 are on.
 *
*/
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "automaton.h"
#include "rules.h"
#include "initial_state.h"
#include "serial.h"
#include "config.h"

const uint32_t neighbors[MY_CELL_COUNT] = MY_NEIGHBORS;

/*
 *	Initialize the state of the petridish.  Sets size based on passed value.
 *	Also assigns neighbors based on values in config.h
*/
void automaton_init(petridish_t *petridish, uint8_t size) {
	petridish->size = size;
	petridish->cells = (cell_t *)malloc(petridish->size * sizeof(cell_t));
	if (petridish->cells == NULL)
		return;

	// Set the cells' initial states
	uint32_t initial_state = initial_state_read();
	//uint32_t initial_state = MY_INITIAL_STATE;
	for (uint8_t i = 0; i < petridish->size; i++) {
		petridish->cells[i].state =
			(initial_state & ((uint32_t)1 << i)) ? ON : OFF ;
		petridish->cells[i].neighbors = neighbors[i];
	}
}

/*
 * Deallocate the malloc'ed memory and cast cell pointer to NULL.
*/
void automaton_destroy(petridish_t *petridish) {
	free(petridish->cells);
	petridish->cells = NULL;
}

/*
	Update cells based on provided rules. Output over serial.
*/
void automaton_update(petridish_t *petridish) {
	for (uint8_t i = 0; i < petridish->size; i++) {
	

		uint8_t n = 0;
		for (uint8_t j = 0; j < petridish->size; j++) {
			if ( (petridish->cells[i].neighbors >> j) & 1)
				if (petridish->cells[j].state == ON)
					n++;
		}

		// Survival Rules
		if (petridish->cells[i].state == ON) {
			if ( (petridish->rules->survival >> n) & 1 )
				petridish->cells[i].state_next = ON;
			else
				petridish->cells[i].state_next = OFF;
		}
		// Birth Rules
		else if (petridish->cells[i].state == OFF) {
			if ( (petridish->rules->birth >> n) & 1 )
				petridish->cells[i].state_next = ON;
			else
				petridish->cells[i].state_next = OFF;
		}
	}
	// Update state and calculate number of living cells
	for (uint8_t i = 0; i < petridish->size; i++) {
		petridish->cells[i].state = petridish->cells[i].state_next;
		serial_write('0'+petridish->cells[i].state);
	}
	serial_write('\r');
	serial_write('\n');
}
