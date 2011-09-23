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
#include "serial.h"
#include "config.h"

/* Create the cells and set initial conditions */
void automaton_init(petridish_t *petridish) {

    petridish->size = MY_CELL_COUNT;
    petridish->cells = (cell_t *)malloc(petridish->size * sizeof(cell_t));
    if (petridish->cells == NULL)
        return;

    // Set the cells' initial states
    //uint32_t initial_state = initial_state_read();
    uint32_t initial_state = MY_INITIAL_STATE;
    for (uint8_t i = 0; i < petridish->size; i++) {
        petridish->cells[i].state =
            (initial_state & ((uint32_t)1 << i)) ? ON : OFF ;
        petridish->cells[i].neighbors = MY_NEIGHBORS[i];
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
        // Write the automaton state to serial out MSB first
        serial_write(petridish->cells[i].state+'0');
    }
    serial_write('\r');
    serial_write('\n');
}
