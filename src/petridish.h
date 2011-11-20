/*
	Declarations relating to the simulation of the cellular petridish.
*/
#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "rules.h"

#define ON  1
#define OFF 0

// Each instance of a cell holds current state, an id, and the neighbors
struct cell_struct {
	uint32_t neighbors; // neighbors bitfield
	uint8_t state;
};
typedef struct cell_struct cell_t;

// Data about the "petridish" the simulation takes place in.
struct petridish_struct;
typedef struct petridish_struct petridish_t;
struct petridish_struct {
	uint8_t size;
	cell_t *cells;
	rules_t *rules;
	void (*update)(petridish_t *petridish);
	void (*destroy)(petridish_t *petridish);
	uint32_t (*get_state)(petridish_t *petridish);
};

// functions for use with the petridish
petridish_t* new_petridish(void);
void petridish_destroy(petridish_t *petridish);
void petridish_update(petridish_t *petridish);
uint32_t petridish_get_state(petridish_t *petridish);
