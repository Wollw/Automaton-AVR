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
#include "automaton.h"
#include "rules.h"
#include "leds.h"

int main(void){
    leds_shift_init();
    rules_shift_init();

    rules_t rules;
    rules_read(&rules);

    petridish_t petridish;
    petridish.size = 24;
    petridish.rules = &rules;

    automaton_init(&petridish);
    
    leds_update(&petridish);
/*
    while (1) {

        petridish_update(&petridish);
        leds_update(&petridish);
        _delay_ms(1000);

    }
*/
    automaton_destroy(&petridish);

    return 0;
}


