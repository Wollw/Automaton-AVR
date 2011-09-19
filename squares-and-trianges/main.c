/*
    This program simulates a Celullar Automaton with an AVR MCU connected to:
        (2) 4021 parallel->serial shift registers for reading rules from dip switches
        (3) more 4021s used to read initial cell state from
        (3) 74HC595 for controlling 24 different LEDs

*/

#include <avr/io.h>
#include <util/delay.h>
#include "rules.h"
#include "leds.h"

int main(void){
    led_shift_init();
    rules_shift_init();

    while (1) {

        uint16_t rules = read_rules();
        update_leds(rules,24);

    }

    return 0;
}


