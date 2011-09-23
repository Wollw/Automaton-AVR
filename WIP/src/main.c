/*
    This program simulates a Celullar Automaton with an AVR MCU connected to:
        (2) 4021 parallel->serial shift registers for reading
            survival rules from dip switches
        (2) 4021 parallel->serial shift registers for reading
            birth rules from dip switches
        (3) 4021 used to read initial cell state from
        (3) 74HC595 for controlling 24 different LEDs

*/

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "automaton.h"
#include "rules.h"
#include "leds.h"

int main(void){
    // Turn the serial port on
    serial_init();

    // Initialize the shift registers connected to
    // the switches used to configure the rules
    // and the initial state.
    leds_shift_init();
    rules_shift_init();

    // Read the configuration information
    // from the switches and initialize the
    // automaton
    rules_t rules;
    rules_read(&rules);
    petridish_t petridish;
    petridish.rules = &rules;
    automaton_init(&petridish);

    // Start running the simulation
    leds_update(&petridish);
    _delay_ms(1000);
    while (1) {

        automaton_update(&petridish);
        leds_update(&petridish);
        _delay_ms(500);

    }

    // Free the memory used by the cells in the petridish.
    // Probably never called but it's here for good form.
    automaton_destroy(&petridish);

    return 0;
}


