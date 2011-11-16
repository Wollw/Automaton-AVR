/*	Automaton AVR - petridish.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	The functions used to change the state of the petridish are defined here.
 *
 *	'petridish_new' is used to setup the initial state of the petridish.
 *	returns a pointer to the petridish.
 *
 *	'petridish_destroy' simply frees the memory that was malloc'ed.
 *
 *	'petridish_update' is the meat of the simulator.  It checks the rules
 *	and applies them to each cell in the petridish.  After getting the next
 *	state of each cell it makes the next state their current state.  It also
 *	outputs their new state over the serial port.
 *
 *	'petridish_get_state' converts and returns the cell states as a int32_t.
 *
 *	Format for serial out starts with bit 0 on the far LEFT of the screen.
 *	Example: '011' means cell 0 is off, and cells 1 and 2 are on.
 *
*/
#include "rules.h"
#include "petridish.h"
#include "initial_state.h"
#include "config.h"

const uint32_t AUTOMATON_NEIGHBORS[] = CONFIG_NEIGHBORS;

/*
 *	Create and return an petridish.  Sets size, neighbors, rules,
 *	initial state, and needed functions before returning a pointer to
 *	the new petridish.
*/
petridish_t* new_petridish() {

	// Create new petridish
	petridish_t *petridish = (petridish_t*)malloc(sizeof(petridish_t));

	// Set number of cells in petridish
	petridish->size =
		sizeof(AUTOMATON_NEIGHBORS) / sizeof(AUTOMATON_NEIGHBORS[0]);

	// Allocate memory for cells
	petridish->cells = (cell_t *)malloc(petridish->size * sizeof(cell_t));
	if (petridish->cells == NULL)
		return NULL;

	// Set the cells' initial states
	uint32_t initial_state = initial_state_read();
	for (uint8_t i = 0; i < petridish->size; i++) {
		petridish->cells[i].state =
			(initial_state & ((uint32_t)1 << i)) ? ON : OFF ;
		petridish->cells[i].neighbors = AUTOMATON_NEIGHBORS[i];
	}

	// Obtain the rules for the petridish
	rules_read(petridish->rules);

	// Set the petridish functions
	petridish->update = &petridish_update;
	petridish->destroy = &petridish_destroy;
	petridish->get_state = &petridish_get_state;

	return petridish;
}

/*
 * Deallocate the malloc'ed memory
*/
void petridish_destroy(petridish_t *petridish) {
	free(petridish->cells);
	petridish->cells = NULL;
	free(petridish);
	petridish = NULL;
}

/*
	Update cells based on provided rules. Output over serial.
*/
void petridish_update(petridish_t *petridish) {

	uint8_t new_states[petridish->size];

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
				new_states[i] = ON;
			else
				new_states[i] = OFF;
		}

		// Birth Rules
		else if (petridish->cells[i].state == OFF) {
			if ( (petridish->rules->birth >> n) & 1 )
				new_states[i] = ON;
			else
				new_states[i] = OFF;
		}
	}

	// Update state and calculate number of living cells
	for (uint8_t i = 0; i < petridish->size; i++) {
		petridish->cells[i].state = new_states[i];
	}
}

/*
	Converts the cell states from the petridish into individual bits.
*/
uint32_t petridish_get_state(petridish_t *petridish) {
	uint32_t state = 0;

	for (uint8_t i = 0; i < petridish->size; i++) {
		if ( petridish->cells[i].state == ON )
			state |= (uint32_t)1 << i;
		else
			state &= ~((uint32_t)1 << i);
	}

	return state;
}
