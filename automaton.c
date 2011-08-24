/*      Copyright David Shere <david.e.shere@gmail.com> 2011
 *  A cellular automaton simulator written for the AVR micro controllers.
 *  Developed for the ATMega328p in particular but should work on others
 *  with no or little modification.
 */
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

// 8MHz 
#define F_CPU 8000000
#define OFF 0
#define ON  1

// ioinit and delay_ms taken from a SparkFun example by Nathan Seidle
// Define functions
void ioinit(void);            //Initializes IO
void delay_ms(uint16_t x); //General purpose delay
struct cell *createCells(uint16_t cellCount); // create cellCount cells
void setCellState(struct cell *myCell,  // Change the cell state to 'state'
                  uint8_t state);       // 0 = off, anything else = on

// Each instance of a cell holds current state, an id, and the neihbours
struct cell {
    uint16_t neighborCount;
    unsigned state :1;
    unsigned stateNext :1;
    uint16_t pin;
    uint16_t *neighbors;
};

int main (void)
{
    ioinit(); //Setup IO pins and defaults

    uint16_t cellCount = 9;
    struct cell *cells = createCells(cellCount);

    int i;
    for (i = 0; i < cellCount; i++)
        setCellState(&cells[i],OFF);
    setCellState(&cells[3],ON);
    setCellState(&cells[4],ON);
    setCellState(&cells[5],ON);

    // Display initial state
    for (i = 0; i < 9; i++) {
        switch (cells[i].state) {
            case 0:
                if (i < 5)
                    PORTB &= ~_BV(cells[i].pin);
                else
                    PORTC &= ~_BV(cells[i].pin);
                break;
            case 1:
                if (i < 5)
                    PORTB |= _BV(cells[i].pin);
                else
                    PORTC |= _BV(cells[i].pin);
                break;
        }
    }
    delay_ms(1000);

    while(1) {
        // Calculate next cell states
        for (i = 0; i < cellCount; i++) {
            int j;
            int n = 0;
            for (j = 0; j < cells[i].neighborCount; j++) {
                n += cells[cells[i].neighbors[j]].state;
            }

            if (cells[i].state == 1 && (n == 2 || n == 3)) {
                cells[i].stateNext = 1;
            }
            else if (cells[i].state == 0 && n == 3) {
                cells[i].stateNext = 1;
            }
            else {
                cells[i].stateNext = 0;
            }
        }

        // Update state
        for (i = 0; i < cellCount; i++) {
            cells[i].state = cells[i].stateNext;
        }

        // Display new state
        for (i = 0; i < cellCount; i++) {
            switch (cells[i].state) {
                case 0:
                    if (i < 5)
                        PORTB &= ~_BV(cells[i].pin);
                    else
                        PORTC &= ~_BV(cells[i].pin);
                    break;
                case 1:
                    if (i < 5)
                        PORTB |= _BV(cells[i].pin);
                    else
                        PORTC |= _BV(cells[i].pin);
                    break;
            }
        }
        delay_ms(1000);
    }

    // Free the allocated memory even though we should never get here
    for (i = 0; i < cellCount; i++) {
        free(cells[i].neighbors);
        cells[i].neighbors = NULL;
    }
    free(cells);
     
    return(0);
}

void ioinit (void)
{
    //1 = output, 0 = input
    DDRB = 0b11111111; //All outputs
    DDRC = 0b11111111; //All outputs
    DDRD = 0b11111110; //PORTD (RX on PD0)
}

//General short delays
void delay_ms(uint16_t x)
{
    uint8_t y, z;
    for ( ; x > 0 ; x--){
        for ( y = 0 ; y < 90 ; y++){
            for ( z = 0 ; z < 6 ; z++){
                asm volatile ("nop");
            }
        }
    }
}

/*
*   Setup the cells to be used in the automaton
*   cellCount : number of cells to reserve space in sram for 
*/
struct cell* createCells(uint16_t cellCount) {
    struct cell *myCells;
    myCells = (struct cell *)malloc(cellCount*sizeof(struct cell));

    myCells[0].pin = PB1;
    myCells[0].neighbors = (uint16_t *)malloc(3*sizeof(uint16_t));
    myCells[0].neighbors[0] = 3;
    myCells[0].neighbors[1] = 4;
    myCells[0].neighbors[2] = 1;
    myCells[0].neighborCount = 3;

    myCells[1].pin = PB2;
    myCells[1].neighbors = (uint16_t *)malloc(5*sizeof(uint16_t));
    myCells[1].neighbors[0] = 3;
    myCells[1].neighbors[1] = 4;
    myCells[1].neighbors[2] = 5;
    myCells[1].neighbors[3] = 0;
    myCells[1].neighbors[4] = 2;
    myCells[1].neighborCount = 5;

    myCells[2].pin = PB3;
    myCells[2].neighbors = (uint16_t *)malloc(3*sizeof(uint16_t));
    myCells[2].neighbors[0] = 4;
    myCells[2].neighbors[1] = 5;
    myCells[2].neighbors[2] = 1;
    myCells[2].neighborCount = 3;

    myCells[3].pin = PB4;
    myCells[3].neighbors = (uint16_t *)malloc(5*sizeof(uint16_t));
    myCells[3].neighbors[0] = 4;
    myCells[3].neighbors[1] = 0;
    myCells[3].neighbors[2] = 1;
    myCells[3].neighbors[3] = 6;
    myCells[3].neighbors[4] = 7;
    myCells[3].neighborCount = 5;

    myCells[4].pin = PB5;
    myCells[4].neighbors = (uint16_t *)malloc(8*sizeof(uint16_t));
    myCells[4].neighbors[0] = 3;
    myCells[4].neighbors[1] = 5;
    myCells[4].neighbors[2] = 0;
    myCells[4].neighbors[3] = 1;
    myCells[4].neighbors[4] = 2;
    myCells[4].neighbors[5] = 6;
    myCells[4].neighbors[6] = 7;
    myCells[4].neighbors[7] = 8;
    myCells[4].neighborCount = 8;

    myCells[5].pin = PC0;
    myCells[5].neighbors = (uint16_t *)malloc(5*sizeof(uint16_t));
    myCells[5].neighbors[0] = 4;
    myCells[5].neighbors[1] = 1;
    myCells[5].neighbors[2] = 2;
    myCells[5].neighbors[3] = 7;
    myCells[5].neighbors[4] = 8;
    myCells[5].neighborCount = 5;

    myCells[6].pin = PC1;
    myCells[6].neighbors = (uint16_t *)malloc(3*sizeof(uint16_t));
    myCells[6].neighbors[0] = 3;
    myCells[6].neighbors[1] = 4;
    myCells[6].neighbors[2] = 7;
    myCells[6].neighborCount = 3;

    myCells[7].pin = PC2;
    myCells[7].neighbors = (uint16_t *)malloc(5*sizeof(uint16_t));
    myCells[7].neighbors[0] = 3;
    myCells[7].neighbors[1] = 4;
    myCells[7].neighbors[2] = 5;
    myCells[7].neighbors[3] = 6;
    myCells[7].neighbors[4] = 8;
    myCells[7].neighborCount = 5;

    myCells[8].pin = PC3;
    myCells[8].neighbors = (uint16_t *)malloc(3*sizeof(uint16_t));
    myCells[8].neighbors[0] = 4;
    myCells[8].neighbors[1] = 5;
    myCells[8].neighbors[2] = 7;
    myCells[8].neighborCount = 3;

    return myCells;
}

void setCellState(struct cell *myCell, uint8_t state) {
    myCell->state = state;
}
