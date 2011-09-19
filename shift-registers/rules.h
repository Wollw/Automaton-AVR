// Parallel->Serial for Rules DIP switches
#define RULES_CP    PD2 // Clock
#define RULES_PS    PD3 // Latch
#define RULES_DO    PD4 // Data Out

/*
    Functions for interacting with the rules.
    See rules.c for details
*/
void rules_shift_init(void);
uint16_t read_rules(void);

