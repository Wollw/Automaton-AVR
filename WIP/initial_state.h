#pragma once
/*
    For reading the initial state of the cell from the dip switches
*/

#define INITIAL_STATE_SHIFT_REG_COUNT   3
#define INITIAL_STATE_PORT              PORTC
#define INITIAL_STATE_PINS              PINC
#define INITIAL_STATE_CP                PIN0
#define INITIAL_STATE_PS                PIN1
#define INITIAL_STATE_DO                PIN2

uint32_t initial_state_read(void);
