#pragma once
#include <stdlib.h>
#include <stdint.h>
/*
	For reading the initial state of the cell from the dip switches
*/

#ifdef CONFIG_USE_SWITCHES_FOR_SETTINGS

#define INITIAL_STATE_SHIFT_REG_COUNT	3
#define	INITIAL_STATE_DDR				DDRC
#define INITIAL_STATE_PORT				PORTC
#define INITIAL_STATE_PINS				PINC
#define INITIAL_STATE_CP				PC0
#define INITIAL_STATE_PS				PC1
#define INITIAL_STATE_DO				PC2

#endif

void initial_state_shift_init(void);
uint32_t initial_state_read(void);
