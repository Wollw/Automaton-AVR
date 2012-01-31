/*	Automaton AVR - rules.c
 *
 *	(c) 2011 - David Ellis Shere
 *  Functions used for handling the rules of the automaton
 *   
 *  rules_shift_init
 *		setup the pins needed for reading the rules
 *		from the dip switches through the 4021 shift
 *		registers
 *
 *	rules_read
 *		takes a rule set as to read the rules of and calls rules_read_dip
 *		to do the actual reading.
 *
 *	rules_read_dip
 *		read the rules dip switches through the 4021 parallel->serial
 *		shift registers.
 *   
*/
#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "rules.h"

/*
	Initialize the pins used for the 4021 shift register
	for use for reading the rules switches
*/
#ifdef	CONFIG_USE_SWITCHES_FOR_SETTINGS
void rules_shift_init(void) {

	/* Survival Rules */
	// Latch and Clock set to output
	RULES_DDR |= _BV(RULES_SURV_PS) | _BV(RULES_SURV_CP);
	// Data In as input
	RULES_DDR &= ~_BV(RULES_SURV_DO);

	/* Birth Rules */
	// Latch and Clock set to output
	RULES_DDR |= _BV(RULES_BIRTH_PS) | _BV(RULES_BIRTH_CP);
	// Data In as input
	RULES_DDR &= ~_BV(RULES_BIRTH_DO);

}
#endif

/*
	This function handles the calls to rules_read and puts the
	rules returned into a provided rules_t variable.
*/
void rules_read(rules_t *rules) {
	// Use hard coded values if no switches present
	#ifdef CONFIG_USE_SWITCHES_FOR_SETTINGS
	rules->survival = rules_read_dip(RULES_SURVIVAL);
	rules->birth = rules_read_dip(RULES_BIRTH);
	#else
	rules->survival = CONFIG_RULES_SURV;
	rules->birth = CONFIG_RULES_BIRTH;
	#endif
}

/*
	This fuction reads the status of the dip switches
	used for rules by shifting in their values with a 4021
	shift register. Returns the rules as a 16 bit value each
	bit of which represents a switch state. bit 0 is switch 1.
*/
#ifdef CONFIG_USE_SWITCHES_FOR_SETTINGS
uint32_t rules_read_dip(uint8_t rules_type) {
	uint32_t rules = 0;
	uint8_t cp, ps, dout; // Clock, P/S and Data Out pins
	
	// Set pin numbers for rule type we want to get
	if (rules_type == RULES_SURVIVAL) {
		cp = RULES_SURV_CP;
		ps = RULES_SURV_PS;
		dout = RULES_SURV_DO;
	} else if (rules_type == RULES_BIRTH) {
		cp = RULES_BIRTH_CP;
		ps = RULES_BIRTH_PS;
		dout = RULES_BIRTH_DO;
	} else {
		return 0;
	}

	/* Read the rules as bits into a uint16_t */
	RULES_PORT |= _BV(ps);
	_delay_us(20);
	RULES_PORT &= ~_BV(ps);
	for (uint8_t i = 0; i < RULES_SHIFT_REG_COUNT * 8; i++) {
		_delay_us(2);

		if ( RULES_PIN & _BV(dout) ) {
			rules |= (uint16_t)1 << i;
		} 

		_delay_us(20);
		RULES_PORT &= ~_BV(cp);
		RULES_PORT |= _BV(cp);
	}

	return rules;
} 
#endif
