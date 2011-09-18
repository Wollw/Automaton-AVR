/*      Copyright David Shere <david.e.shere@gmail.com> 2011
 *  A cellular automaton simulator written for the AVR micro controllers.
 *  Developed for the ATMega328p in particular but should work on others
 *  with no or little modification.
 */
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "automaton.h"
#include "config.h"

#define MHZ 1
#define OFF 0
#define ON  1
#define CELL_COUNT 21

int main (void)
{
    // Turn IO pins on
    ioinit();

    // Blink all LEDs to make sure they all work
/*    int i = 100;
    for (; i > 0; i--) {
        blinkLEDs(i);
    }
    delay_ms(1000);
*/
    struct ruleStruct rules = { RULES_SURVIVAL, RULES_BIRTH };
    struct cellStruct *cells = createCells(INITIAL_STATE);
    if (cells == NULL)
        while (1)
            blinkLEDs(100);

    
    uint16_t delay = DELAY_MS;
    while(1) {
        #if STAGGER_SPEED
            delay = getCellCount(cells,ON) * STAGGER_DELAY_MS;
        #endif

        displayBoard(cells);

        applyRules(cells,&rules);

        delay_ms(delay);
    }

    // Free the allocated memory even though we should never get here
    if (cells != NULL)
        free(cells);
    cells = NULL;
     
    return(0);
}

void ioinit (void)
{
    // Data Direction Registers
    // 0 = input; 1 = output
    DDRB  = 0b11111111;
    DDRC  = 0b00111111;
    DDRD  = 0b01111111;

    PORTB = 0b00000000;
    PORTC = 0b11000000;
    PORTD = 0b10000000;
}

//General short delays - modified by David Shere to add MHZ loop
void delay_ms(uint16_t x)
{
    uint8_t w, y, z;
    for ( ; x > 0 ; x--){
        for ( w = 0; w < MHZ; w++) {
            for ( y = 0 ; y < 90 ; y++){
                for ( z = 0 ; z < 6 ; z++){
                    asm volatile ("nop");
                }
            }
        }
    }
}

/*
*   Setup the cells to be used in the automaton
*   neighbors bit numbers are equal to the cells' indices
*/
struct cellStruct *createCells(uint32_t initialState) {
    struct cellStruct *myCells;
    myCells = (struct cellStruct *)malloc(CELL_COUNT*sizeof(struct cellStruct));
    if (myCells == NULL)
        return NULL;

    myCells[0].port = (uint8_t*)&PORTB;
    myCells[0].pin = PB0;
    myCells[0].neighbors = 0b11111111110;

    myCells[1].port = (uint8_t*)&PORTB;
    myCells[1].pin = PB1;
    myCells[1].neighbors = 0b100001100011100101;

    myCells[2].port = (uint8_t*)&PORTB;
    myCells[2].pin = PB2;
    myCells[2].neighbors = 0b1000011000110001011;

    myCells[3].port = (uint8_t*)&PORTB;
    myCells[3].pin = PB3;
    myCells[3].neighbors = 0b10000110001100010101;

    myCells[4].port = (uint8_t*)&PORTB;
    myCells[4].pin = PB4;
    myCells[4].neighbors = 0b100001100011000101001;

    myCells[5].port = (uint8_t*)&PORTB;
    myCells[5].pin = PB5;
    myCells[5].neighbors = 0b11000110001010011;

    myCells[6].port = (uint8_t*)&PORTB;
    myCells[6].pin = PB6;
    myCells[6].neighbors = 0b111001110010100011;

    myCells[7].port = (uint8_t*)&PORTC;
    myCells[7].pin = PC2;
    myCells[7].neighbors = 0b1100011100101000111;

    myCells[8].port = (uint8_t*)&PORTC;
    myCells[8].pin = PC1;
    myCells[8].neighbors = 0b11000111001010001101;

    myCells[9].port = (uint8_t*)&PORTC;
    myCells[9].pin = PC0;
    myCells[9].neighbors = 0b110001110010100011001;

    myCells[10].port = (uint8_t*)&PORTB;
    myCells[10].pin = PB7;
    myCells[10].neighbors = 0b100011100101001110001;

    myCells[11].port = (uint8_t*)&PORTC;
    myCells[11].pin = PC3;
    myCells[11].neighbors = 0b111001010011100010;

    myCells[12].port = (uint8_t*)&PORTD;
    myCells[12].pin = PD1;
    myCells[12].neighbors = 0b1100010100111000110;

    myCells[13].port = (uint8_t*)&PORTD;
    myCells[13].pin = PD0;
    myCells[13].neighbors = 0b11000101001110001100;

    // Skipping PC6 as it is the reset pin
    // There is no PC7 on the ATMega328P

    myCells[14].port = (uint8_t*)&PORTC;
    myCells[14].pin = PC5;
    myCells[14].neighbors = 0b110001010011100011000;

    myCells[15].port = (uint8_t*)&PORTC;
    myCells[15].pin = PC4;
    myCells[15].neighbors = 0b100010100111001110000;

    myCells[16].port = (uint8_t*)&PORTD;
    myCells[16].pin = PD3;
    myCells[16].neighbors = 0b100101000110001100000;

    myCells[17].port = (uint8_t*)&PORTD;
    myCells[17].pin = PD2;
    myCells[17].neighbors = 0b1010001100011000010;

    myCells[18].port = (uint8_t*)&PORTD;
    myCells[18].pin = PD6;
    myCells[18].neighbors = 0b10100011000110000100;

    myCells[19].port = (uint8_t*)&PORTD;
    myCells[19].pin = PD5;
    myCells[19].neighbors = 0b101000110001100001000;

    myCells[20].port = (uint8_t*)&PORTD;
    myCells[20].pin = PD4;
    myCells[20].neighbors = 0b10011100011000010000;

    uint8_t i;
    for (i = 0; i < CELL_COUNT; i++)
        if ( (initialState >> i) & 1)
            myCells[i].state = ON;
        else
            myCells[i].state = OFF;

    return myCells;
}

// Takes an array of cells "myCells" turns LEDs on and off to simulate life/death
void displayBoard(struct cellStruct *myCells) {
    uint8_t i;
    for (i = 0; i < CELL_COUNT; i++) {
        switch (myCells[i].state) {
            case ON:
                *(myCells[i].port) |= _BV(myCells[i].pin);
                break;
            case OFF:
                *(myCells[i].port) &= ~_BV(myCells[i].pin);
                break;
        }
    }
}

// Blink LEDs on and off for a total of 'ms' milliseconds
void blinkLEDs(uint16_t ms) {
    PORTB = 0xFF;
    PORTC = 0x3F;
    PORTD = 0x7F;
    delay_ms(ms/2);
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    delay_ms(ms/2);
}

// Apply the rules of the game to the cells
void applyRules(struct cellStruct *myCells, struct ruleStruct *myRules) {
    uint8_t i;
    for (i = 0; i < CELL_COUNT; i++) {
        uint8_t n = 0;
        uint8_t j;
        for (j = 0; j < CELL_COUNT; j++) {
            if ( (myCells[i].neighbors >> j) & 1)
                if (myCells[j].state == ON)
                    n++;
        }

        // Survival Rules
        if (myCells[i].state == ON) {
            if ( (myRules->survival >> n) & 1 )
                myCells[i].stateNext = ON;
            else
                myCells[i].stateNext = OFF;
        }
        // Birth Rules
        else if (myCells[i].state == OFF) {
            if ( (myRules->birth >> n) & 1 )
                myCells[i].stateNext = ON;
            else
                myCells[i].stateNext = OFF;
        }
        else { // Error
            while (1) {
                blinkLEDs(100);
            }
        }
    }
    // Update state and calculate number of living cells
    for (i = 0; i < CELL_COUNT; i++) {
        myCells[i].state = myCells[i].stateNext;
    }
}

// get the number of living/dead cells
uint8_t getCellCount(struct cellStruct *myCells, uint8_t state) {
    uint8_t i;
    uint8_t c = 0;
    for (i = 0; i < CELL_COUNT; i++) {
        if (myCells[i].state == state)
            c++;
    }
    return c;
}
