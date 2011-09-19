/*
*   Functions used for handling the rules of the automaton
*   
*   rules_shift_init
*       setup the pins needed for reading the rules
*       from the dip switches through the 4021 shift
*       registers
*
*   read_rules
*       read the rules dip switches through the 4021 parallel->serial
*       shift registers.
*/
#include <avr/io.h>
#include <util/delay.h>
#include "rules.h"

/*
    Initialize the pins used for the 4021 shift register
    for use for reading the rules switches
*/
void rules_shift_init(void) {
    // Latch and Clock set to output
    DDRD |= _BV(RULES_PS) | _BV(RULES_CP);
    // Data In as input
    DDRD &= ~_BV(RULES_DO);
}

/*
    This fuction reads the status of the dip switches
    used for rules by shifting in their values with a 4021
    shift register. Returns the rules as a 16 bit value each
    bit of which represents a switch state. bit 0 is switch 1.
*/
uint16_t read_rules() {
    uint16_t rules = 0;

    PORTD |= _BV(RULES_PS);
    _delay_us(20);
    PORTD &= ~_BV(RULES_PS);

    int i;
    for (i = 0; i < 16; i++) {
        _delay_us(2);

        if ( PIND & _BV(RULES_DO) )
            rules |= (uint16_t)1 << i;
        _delay_us(20);
        PORTD &= ~_BV(RULES_CP);
        PORTD |= _BV(RULES_CP);
    }
    return rules;
} 
