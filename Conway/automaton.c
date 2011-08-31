/*      Copyright David Shere <david.e.shere@gmail.com> 2011
 *  A cellular automaton simulator written for the AVR micro controllers.
 *  Developed for the ATMega328p in particular but should work on others
 *  with no or little modification.
 */
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#define MHZ 1
#define OFF 0
#define ON  1
#define CELL_COUNT 9

// ioinit and delay_ms taken from a SparkFun example by Nathan Seidle
// Define functions
void ioinit(void);            //Initializes IO
void delay_ms(uint16_t x); //General purpose delay
struct cell *createCells(void); // create CELL_COUNT cells
void setCellState(struct cell *myCell,  // Change the cell state to 'state'
                  uint8_t state);       // 0 = off, anything else = on
void displayBoard(struct cell *myCells); // Display the state of the board with LEDs on IO
void blinkLEDs(uint16_t ms);    // Blink LEDs for a total of 'ms' milliseconds;
void updateCellStates(struct cell *myCells); // Apply the rules of the game to the cells

// Each instance of a cell holds current state, an id, and the neihbours
struct cell {
    uint8_t state;
    uint8_t stateNext;
    uint8_t *port;
    uint8_t pin;
    uint32_t neighbors; // neighbors stored as bits to save ram
};

int main (void)
{
    // Turn IO pins on
    ioinit();

    // Blink all LEDs to make sure they all work
    blinkLEDs(500);
    delay_ms(500);

    struct cell *cells = createCells();

    cells[3].state = ON;
    cells[4].state = ON;
    cells[5].state = ON;

    while(1) {
        // Display the board state with LEDs
        displayBoard(cells);

        updateCellStates(cells);

        // 1 second interval
        delay_ms(1000);
    }

    // Free the allocated memory even though we should never get here
    free(cells);
    cells = NULL;
     
    return(0);
}

void ioinit (void)
{
    //1 = output, 0 = input
    DDRB = 0b11111111;
    DDRC = 0b00111111;
    DDRD = 0b01111111;
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
struct cell* createCells(void) {
    struct cell *myCells;
    myCells = (struct cell *)malloc(CELL_COUNT*sizeof(struct cell));

    myCells[0].port = (uint8_t*)&PORTB;
    myCells[0].pin = PB1;
    myCells[0].neighbors = 0b11010;

    myCells[1].port = (uint8_t*)&PORTB;
    myCells[1].pin = PB2;
    myCells[1].neighbors = 0b111101;

    myCells[2].port = (uint8_t*)&PORTB;
    myCells[2].pin = PB3;
    myCells[2].neighbors = 0b110010;

    myCells[3].port = (uint8_t*)&PORTB;
    myCells[3].pin = PB4;
    myCells[3].neighbors = 0b11010011;

    myCells[4].port = (uint8_t*)&PORTB;
    myCells[4].pin = PB5;
    myCells[4].neighbors = 0b111101111;

    myCells[5].port = (uint8_t*)&PORTC;
    myCells[5].pin = PC0;
    myCells[5].neighbors = 0b110010110;

    myCells[6].port = (uint8_t*)&PORTC;
    myCells[6].pin = PC1;
    myCells[6].neighbors = 0b10011000;

    myCells[7].port = (uint8_t*)&PORTC;
    myCells[7].pin = PC2;
    myCells[7].neighbors = 0b101111000;

    myCells[8].port = (uint8_t*)&PORTC;
    myCells[8].pin = PC3;
    myCells[8].neighbors = 0b10110000;

    uint8_t i;
    for (i = 0; i < CELL_COUNT; i++)
        myCells[i].state = OFF;

    return myCells;
}

// Takes an array of cells "myCells" turns LEDs on and off to simulate life/death
void displayBoard(struct cell *myCells) {
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
void updateCellStates(struct cell *myCells) {
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
            switch (n) {
                case 2:
                case 3:
                    myCells[i].stateNext = ON;
                    break;
                default:
                    myCells[i].stateNext = OFF;
                    break;
            }
        }
        // Birth Rules
        else if (myCells[i].state == OFF) {
            switch (n) {
                case 3:
                    myCells[i].stateNext = ON;
                    break;
                default:
                    myCells[i].stateNext = OFF;
                    break;
            }
        }
        else {
            blinkLEDs(100);
            blinkLEDs(100);
            blinkLEDs(100);
            blinkLEDs(100);
            blinkLEDs(100);
        }
    }
    // Update state
    for (i = 0; i < CELL_COUNT; i++) {
        myCells[i].state = myCells[i].stateNext;
    }
}
