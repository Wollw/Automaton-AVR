#pragma once
/*
	Declarations relating to the simulation of the cellular automaton.
*/
#include "rules.h"

#define ON  1
#define OFF 0

// Each instance of a cell holds current state, an id, and the neighbors
struct cell_struct {
	uint8_t state;
	uint32_t neighbors; // neighbors bitfield
};
typedef struct cell_struct cell_t;

// Data about the "automaton" the simulation takes place in.
struct automaton_struct;
typedef struct automaton_struct automaton_t;
struct automaton_struct {
	uint8_t size;
	cell_t *cells;
	rules_t *rules;
	void (*update)(automaton_t *automaton);
	void (*destroy)(automaton_t *automaton);
	void (*display)(automaton_t *automaton);
	void (*delay)(void);
};

// functions for use with the automaton
automaton_t* automaton_new(void);
void automaton_destroy(automaton_t *automaton);
void automaton_update(automaton_t *automaton);
void automaton_delay(void);
void automaton_display(automaton_t *automaton);
