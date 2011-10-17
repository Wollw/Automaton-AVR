/*
*   Functions used for handling the rules of the automaton
*   
*   rules_shift_init
*	   setup the pins needed for reading the rules
*	   from the dip switches through the 4021 shift
*	   registers
*
*   rules_read
*	   read the rules dip switches through the 4021 parallel->serial
*	   shift registers.
*   
*   rules_save_eeprom
*	   save the current rules to eeprom.  checks bit 13 (last switch on
*	   the rule dip switches, furthest right) for rules set and then
*	   checks to see if eeprom needs to be updated.  Only actually writes
*	   if rules differ from eeprom.
*/
#include <avr/io.h>
#include <util/delay.h>
#include "rules.h"

#define NULL 0

/*
	Initialize the pins used for the 4021 shift register
	for use for reading the rules switches
*/
void rules_shift_init(void) {
	// Survival Rules
	// Latch and Clock set to output
	RULES_DDR |= _BV(RULES_SURV_PS) | _BV(RULES_SURV_CP);
	// Data In as input
	RULES_DDR &= ~_BV(RULES_SURV_DO);

	// Birth Rules
	// Latch and Clock set to output
	RULES_DDR |= _BV(RULES_BIRTH_PS) | _BV(RULES_BIRTH_CP);
	// Data In as input
	RULES_DDR &= ~_BV(RULES_BIRTH_DO);
}

/*
	This function handles the calls to rules_read and puts the
	rules returned into a provided rules_t variable.
*/
void rules_read(rules_t *rules) {
	rules->survival = rules_read_dip(RULES_SURVIVAL);
	rules->birth = rules_read_dip(RULES_BIRTH);
}

/*
	This fuction reads the status of the dip switches
	used for rules by shifting in their values with a 4021
	shift register. Returns the rules as a 16 bit value each
	bit of which represents a switch state. bit 0 is switch 1.
*/
uint16_t rules_read_dip(uint8_t rules_type) {
	uint16_t rules = 0;
	uint8_t cp, ps, dout; // Clock, P/S and Data Out pins
	
	// Get pin numbers for rule type we want to get
	if (rules_type == RULES_SURVIVAL) {
		cp = RULES_SURV_CP;
		ps = RULES_SURV_PS;
		dout = RULES_SURV_DO;
	} else if (rules_type == RULES_BIRTH) {
		cp = RULES_BIRTH_CP;
		ps = RULES_BIRTH_PS;
		dout = RULES_BIRTH_DO;
	} else {
		cp = NULL;
		ps = NULL;
		dout = NULL;
	}

	RULES_PORT |= _BV(ps);
	_delay_us(20);
	RULES_PORT &= ~_BV(ps);

	for (uint8_t i = 0; i < RULES_SHIFT_REG_COUNT * 8; i++) {
		_delay_us(2);

		if ( RULES_PIN & _BV(dout) )
			rules |= (uint16_t)1 << i;
		_delay_us(20);
		RULES_PORT &= ~_BV(cp);
		RULES_PORT |= _BV(cp);
	}
	return rules;
} 
