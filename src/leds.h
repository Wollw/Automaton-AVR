#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "petridish.h"

// shift registers (74HC595) for LEDs
#define LEDS_SHIFT_REG_COUNT	3

// Serial->Parallel for LEDs - Might change to PORTC to leave SPI open
#define LEDS_DDR	DDRB
#define LEDS_PORT   PORTB
#define LEDS_RCLK   PB0
#define LEDS_SER	PB1
#define LEDS_SRCLK  PB2

// Functions for interacting with the output for leds
// See leds.c for details
void leds_shift_init(void);
void leds_change_state(uint64_t new_state, uint8_t pins_to_shift);
