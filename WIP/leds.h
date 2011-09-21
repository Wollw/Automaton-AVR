#pragma once
#include "automaton.h"

// Number of pins avaiable through serial->parallel
// shift registers (74HC595)
#define SHIFT_OUT_PIN_COUNT 24

// Serial->Parallel for LEDs
#define RCLK    PB0
#define SER     PB1
#define SRCLK   PB2

// Functions for interacting with the output for leds
// See leds.c for details
void leds_shift_init(void);
void leds_update(petridish_t *petridish);
void leds_shift_out(uint32_t new_state, uint8_t pins_to_shift);
