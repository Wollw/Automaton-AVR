#pragma once

// Parallel->Serial for Survival Rules DIP switches
#define RULES_SURV_CP    PD2 // Clock
#define RULES_SURV_PS    PD3 // Latch
#define RULES_SURV_DO    PD4 // Data Out

// Parallel->Serial for Birth Rules DIP switches
#define RULES_BIRTH_CP    PD6 // Clock
#define RULES_BIRTH_PS    PD7 // Latch
#define RULES_BIRTH_DO    PD5 // Data Out

// Number of shift registers used for rules
#define RULES_SHIFT_REG_COUNT 2

// IDs for rule sets
#define RULES_SURVIVAL  1
#define RULES_BIRTH     0

// Structure to hold the game rules
struct rules_struct {
    uint16_t survival;
    uint16_t birth;
};
typedef struct rules_struct rules_t;

/*
    Functions for interacting with the rules.
    See rules.c for details
*/

void rules_shift_init(void);
void rules_read(rules_t *rules);
uint16_t rules_read_dip(uint8_t rules_type);
