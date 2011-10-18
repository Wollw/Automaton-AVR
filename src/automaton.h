#pragma once
/*
	Declarations relating to the simulation of the cellular automaton.
*/
#include "rules.h"

#define ON  1
#define OFF 0

// Each instance of a cell holds current state, an id, and the neighbors
struct cell_struct {
	unsigned state :1;
	unsigned state_next :1;
	uint32_t neighbors; // neighbors bitfield
};
typedef struct cell_struct cell_t;

// Data about the "automaton" the simulation takes place in.
// The game board.
struct automaton_struct;
typedef struct automaton_struct automaton_t;
struct automaton_struct {
	uint8_t size;
	cell_t *cells;
	rules_t *rules;
	void (*init)(automaton_t *automaton);
	void (*update)(automaton_t *automaton);
	void (*destroy)(automaton_t *automaton);
	void (*display)(automaton_t *automaton);
	void (*delay)(void);
};

/*
	Update the state of the cells in the automaton based on the current generation
*/
void automaton_init(automaton_t *automaton);
void automaton_destroy(automaton_t *automaton);
void automaton_update(automaton_t *automaton);
void automaton_delay(void);
void automaton_display(automaton_t *automaton);
