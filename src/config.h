// 3x3 grid

// Include once
#pragma once
#ifndef CONFIGED
#define CONFIGED

//#define CONFIG_USE_SERIAL
//#define	CONFIG_USE_SWITCHES_FOR_SETTINGS
#define CONFIG_RULES_SURV	0b1100
#define CONFIG_RULES_BIRTH	0b1000
#define CONFIG_INITIAL_STATE	0b000111000

// Time each turn lasts (the amount of time a unique state of the
// automaton is displayed)
#define CONFIG_DELAY_MS 1000

/*
 *	An array of neighboring cells for each cell.
 *	Each index is a cell id so that Cell ID #0 has the neighbor
 *	value 0b000011010.  Each bit refers to another cell's ID so that
 *	the first cell (Cell ID #0) with the aforementioned neighbor value of
 *	0b000011010 translates to being neighbors with Cell ID #1, #3, and #4
 *	because bits 1, 3, and 4 are set.
*/
#define CONFIG_NEIGHBORS \
    {                \
        0b000011010, \
        0b000111101, \
        0b000110010, \
        0b011010011, \
        0b111101111, \
        0b110010110, \
        0b010011000, \
        0b101111000, \
        0b010110000, \
		0,0,0,0,0,0,0,0,0,0 \
    }
#endif
