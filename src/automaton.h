#pragma once
/*
    Definitions relating to the simulation of the
    cellular automaton.
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

// Data about the "petridish" the simulation takes place in.
// The game board.
struct petridish_struct {
    uint8_t size;
    rules_t *rules;
    cell_t *cells;
};
typedef struct petridish_struct petridish_t;

/*
    Update the state of the cells in the automaton based on the current generation
*/
void automaton_init(petridish_t *petridish);
void automaton_destroy(petridish_t *petridish);
void automaton_update(petridish_t *petridish);
