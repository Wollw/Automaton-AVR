/*
    Functions relating to the simulation
    of the cellular automaton.
    
    automaton_update
        update an array of cells based on a set of rules
*/
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "automaton.h"
#include "rules.h"
#include "initial_state.h"

/* Create the cells and set initial conditions */
void automaton_init(petridish_t *petridish) {
    petridish->cells = (cell_t *)malloc(petridish->size * sizeof(cell_t));
    if (petridish->cells == NULL)
        return;

    // Set the cells' initial states
    uint32_t initial_state = initial_state_read();
    int i;
    for (i = 0; i < petridish->size; i++) {
        petridish->cells[i].state = (initial_state & ((uint32_t)1 << i)) ? ON : OFF ;
    }
}

/* free the memory allocated for the cells. */
void automaton_destroy(petridish_t *petridish) {
    free(petridish->cells);
    petridish->cells = NULL;
}

/*
    Update cells based on provided rules.
*/
void automaton_update(petridish_t *petridish) {
}    
