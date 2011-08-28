/*      Copyright David Shere <david.e.shere@gmail.com> 2011
 *  A cellular automaton simulator written for the AVR micro controllers.
 *  Developed for the ATMega328p in particular but should work on others
 *  with no or little modification.
 */
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#define MHZ 8
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

    uint16_t cellCount = 21;
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
    cells = NULL;
     
    return(0);
}

void ioinit (void)
{
    //1 = output, 0 = input
    DDRB = 0b11111111; //All outputs
    DDRC = 0b11111111; //All outputs
    DDRD = 0b11111110; //PORTD (RX on PD0)
}

//General short delays - modified by David Shere to add MHZ loop
void delay_ms(uint16_t x)
{
    uint8_t y, z;
    int w;
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
*   cellCount : number of cells to reserve space in sram for 
*/
struct cell* createCells(uint16_t cellCount) {
    struct cell *myCells;
    myCells = (struct cell *)malloc(cellCount*sizeof(struct cell));

    myCells[0].pin = PB0;
    myCells[0].neighborCount = 10;
    myCells[0].neighbors = (uint16_t *)malloc(myCells[0].neighborCount*sizeof(uint16_t));
    myCells[0].neighbors[0] = 1;
    myCells[0].neighbors[1] = 2;
    myCells[0].neighbors[2] = 3;
    myCells[0].neighbors[3] = 4;
    myCells[0].neighbors[4] = 5;
    myCells[0].neighbors[5] = 6;
    myCells[0].neighbors[6] = 7;
    myCells[0].neighbors[7] = 8;
    myCells[0].neighbors[8] = 9;
    myCells[0].neighbors[9] = 10;

    myCells[1].pin = PB1;
    myCells[1].neighborCount = 8;
    myCells[1].neighbors = (uint16_t *)malloc(myCells[1].neighborCount*sizeof(uint16_t));
    myCells[1].neighbors[0] = 0;
    myCells[1].neighbors[1] = 2;
    myCells[1].neighbors[2] = 5;
    myCells[1].neighbors[3] = 6;
    myCells[1].neighbors[4] = 7;
    myCells[1].neighbors[5] = 11;
    myCells[1].neighbors[6] = 12;
    myCells[1].neighbors[7] = 17;

    myCells[2].pin = PB2;
    myCells[2].neighborCount = 8;
    myCells[2].neighbors = (uint16_t *)malloc(myCells[2].neighborCount*sizeof(uint16_t));
    myCells[2].neighbors[0] = 0;
    myCells[2].neighbors[1] = 1;
    myCells[2].neighbors[2] = 3;
    myCells[2].neighbors[3] = 7;
    myCells[2].neighbors[4] = 8;
    myCells[2].neighbors[5] = 12;
    myCells[2].neighbors[6] = 13;
    myCells[2].neighbors[7] = 18;

    myCells[3].pin = PB3;
    myCells[3].neighborCount = 8;
    myCells[3].neighbors = (uint16_t *)malloc(myCells[2].neighborCount*sizeof(uint16_t));
    myCells[3].neighbors[0] = 0;
    myCells[3].neighbors[1] = 2;
    myCells[3].neighbors[2] = 4;
    myCells[3].neighbors[3] = 8;
    myCells[3].neighbors[4] = 9;
    myCells[3].neighbors[5] = 13;
    myCells[3].neighbors[6] = 14;
    myCells[3].neighbors[7] = 19;

    myCells[4].pin = PB4;
    myCells[4].neighborCount = 8;
    myCells[4].neighbors = (uint16_t *)malloc(myCells[4].neighborCount*sizeof(uint16_t));
    myCells[4].neighbors[0] = 0;
    myCells[4].neighbors[1] = 3;
    myCells[4].neighbors[2] = 5;
    myCells[4].neighbors[3] = 9;
    myCells[4].neighbors[4] = 10;
    myCells[4].neighbors[5] = 14;
    myCells[4].neighbors[6] = 15;
    myCells[4].neighbors[7] = 20;

    myCells[5].pin = PB5;
    myCells[5].neighborCount = 8;
    myCells[5].neighbors = (uint16_t *)malloc(myCells[5].neighborCount*sizeof(uint16_t));
    myCells[5].neighbors[0] = 0;
    myCells[5].neighbors[1] = 1;
    myCells[5].neighbors[2] = 4;
    myCells[5].neighbors[3] = 6;
    myCells[5].neighbors[4] = 10;
    myCells[5].neighbors[5] = 11;
    myCells[5].neighbors[6] = 15;
    myCells[5].neighbors[7] = 16;

    myCells[6].pin = PB6;
    myCells[6].neighborCount = 10;
    myCells[6].neighbors = (uint16_t *)malloc(myCells[6].neighborCount*sizeof(uint16_t));
    myCells[6].neighbors[0] = 0;
    myCells[6].neighbors[1] = 1;
    myCells[6].neighbors[2] = 5;
    myCells[6].neighbors[3] = 7;
    myCells[6].neighbors[4] = 10;
    myCells[6].neighbors[5] = 11;
    myCells[6].neighbors[6] = 12;
    myCells[6].neighbors[7] = 15;
    myCells[6].neighbors[8] = 16;
    myCells[6].neighbors[9] = 17;

    myCells[7].pin = PB7;
    myCells[7].neighborCount = 10;
    myCells[7].neighbors = (uint16_t *)malloc(myCells[7].neighborCount*sizeof(uint16_t));
    myCells[7].neighbors[0] = 0;
    myCells[7].neighbors[1] = 1;
    myCells[7].neighbors[2] = 2;
    myCells[7].neighbors[3] = 6;
    myCells[7].neighbors[4] = 8;
    myCells[7].neighbors[5] = 11;
    myCells[7].neighbors[6] = 12;
    myCells[7].neighbors[7] = 13;
    myCells[7].neighbors[8] = 17;
    myCells[7].neighbors[9] = 18;

    myCells[8].pin = PC0;
    myCells[8].neighborCount = 10;
    myCells[8].neighbors = (uint16_t *)malloc(myCells[8].neighborCount*sizeof(uint16_t));
    myCells[8].neighbors[0] = 0;
    myCells[8].neighbors[1] = 2;
    myCells[8].neighbors[2] = 3;
    myCells[8].neighbors[3] = 7;
    myCells[8].neighbors[4] = 9;
    myCells[8].neighbors[5] = 12;
    myCells[8].neighbors[6] = 13;
    myCells[8].neighbors[7] = 14;
    myCells[8].neighbors[8] = 18;
    myCells[8].neighbors[9] = 19;

    myCells[9].pin = PC1;
    myCells[9].neighborCount = 10;
    myCells[9].neighbors = (uint16_t *)malloc(myCells[9].neighborCount*sizeof(uint16_t));
    myCells[9].neighbors[0] = 0;
    myCells[9].neighbors[1] = 3;
    myCells[9].neighbors[2] = 4;
    myCells[9].neighbors[3] = 8;
    myCells[9].neighbors[4] = 10;
    myCells[9].neighbors[5] = 13;
    myCells[9].neighbors[6] = 14;
    myCells[9].neighbors[7] = 15;
    myCells[9].neighbors[8] = 19;
    myCells[9].neighbors[9] = 20;

    myCells[10].pin = PC2;
    myCells[10].neighborCount = 10;
    myCells[10].neighbors = (uint16_t *)malloc(myCells[10].neighborCount*sizeof(uint16_t));
    myCells[10].neighbors[0] = 0;
    myCells[10].neighbors[1] = 4;
    myCells[10].neighbors[2] = 5;
    myCells[10].neighbors[3] = 6;
    myCells[10].neighbors[4] = 9;
    myCells[10].neighbors[5] = 11;
    myCells[10].neighbors[6] = 14;
    myCells[10].neighbors[7] = 15;
    myCells[10].neighbors[8] = 16;
    myCells[10].neighbors[9] = 20;

    myCells[11].pin = PC3;
    myCells[11].neighborCount = 9;
    myCells[11].neighbors = (uint16_t *)malloc(myCells[11].neighborCount*sizeof(uint16_t));
    myCells[11].neighbors[0] = 1;
    myCells[11].neighbors[1] = 5;
    myCells[11].neighbors[2] = 6;
    myCells[11].neighbors[3] = 7;
    myCells[11].neighbors[4] = 10;
    myCells[11].neighbors[5] = 12;
    myCells[11].neighbors[6] = 15;
    myCells[11].neighbors[7] = 16;
    myCells[11].neighbors[8] = 17;

    myCells[12].pin = PC4;
    myCells[12].neighborCount = 9;
    myCells[12].neighbors = (uint16_t *)malloc(myCells[12].neighborCount*sizeof(uint16_t));
    myCells[12].neighbors[0] = 1;
    myCells[12].neighbors[1] = 2;
    myCells[12].neighbors[2] = 6;
    myCells[12].neighbors[3] = 7;
    myCells[12].neighbors[4] = 8;
    myCells[12].neighbors[5] = 11;
    myCells[12].neighbors[6] = 13;
    myCells[12].neighbors[7] = 17;
    myCells[12].neighbors[8] = 18;

    myCells[13].pin = PC5;
    myCells[13].neighborCount = 9;
    myCells[13].neighbors = (uint16_t *)malloc(myCells[13].neighborCount*sizeof(uint16_t));
    myCells[13].neighbors[0] = 2;
    myCells[13].neighbors[1] = 3;
    myCells[13].neighbors[2] = 7;
    myCells[13].neighbors[3] = 8;
    myCells[13].neighbors[4] = 9;
    myCells[13].neighbors[5] = 12;
    myCells[13].neighbors[6] = 14;
    myCells[13].neighbors[7] = 18;
    myCells[13].neighbors[8] = 19;

    myCells[14].pin = PC6;
    myCells[14].neighborCount = 9;
    myCells[14].neighbors = (uint16_t *)malloc(myCells[14].neighborCount*sizeof(uint16_t));
    myCells[14].neighbors[0] = 3;
    myCells[14].neighbors[1] = 4;
    myCells[14].neighbors[2] = 8;
    myCells[14].neighbors[3] = 9;
    myCells[14].neighbors[4] = 10;
    myCells[14].neighbors[5] = 13;
    myCells[14].neighbors[6] = 15;
    myCells[14].neighbors[7] = 19;
    myCells[14].neighbors[8] = 20;

    myCells[15].pin = PD0;
    myCells[15].neighborCount = 9;
    myCells[15].neighbors = (uint16_t *)malloc(myCells[15].neighborCount*sizeof(uint16_t));
    myCells[15].neighbors[0] = 4;
    myCells[15].neighbors[1] = 5;
    myCells[15].neighbors[2] = 6;
    myCells[15].neighbors[3] = 9;
    myCells[15].neighbors[4] = 10;
    myCells[15].neighbors[5] = 11;
    myCells[15].neighbors[6] = 14;
    myCells[15].neighbors[7] = 16;
    myCells[15].neighbors[8] = 20;

    myCells[16].pin = PD1;
    myCells[16].neighborCount = 7;
    myCells[16].neighbors = (uint16_t *)malloc(myCells[16].neighborCount*sizeof(uint16_t));
    myCells[16].neighbors[0] = 5;
    myCells[16].neighbors[1] = 6;
    myCells[16].neighbors[2] = 10;
    myCells[16].neighbors[3] = 11;
    myCells[16].neighbors[4] = 15;
    myCells[16].neighbors[5] = 17;
    myCells[16].neighbors[6] = 20;

    myCells[17].pin = PD2;
    myCells[17].neighborCount = 7;
    myCells[17].neighbors = (uint16_t *)malloc(myCells[17].neighborCount*sizeof(uint16_t));
    myCells[17].neighbors[0] = 1;
    myCells[17].neighbors[1] = 6;
    myCells[17].neighbors[2] = 7;
    myCells[17].neighbors[3] = 11;
    myCells[17].neighbors[4] = 12;
    myCells[17].neighbors[5] = 16;
    myCells[17].neighbors[6] = 18;

    myCells[18].pin = PD3;
    myCells[18].neighborCount = 7;
    myCells[18].neighbors = (uint16_t *)malloc(myCells[18].neighborCount*sizeof(uint16_t));
    myCells[18].neighbors[0] = 2;
    myCells[18].neighbors[1] = 7;
    myCells[18].neighbors[2] = 8;
    myCells[18].neighbors[3] = 12;
    myCells[18].neighbors[4] = 13;
    myCells[18].neighbors[5] = 17;
    myCells[18].neighbors[6] = 19;

    myCells[19].pin = PD4;
    myCells[19].neighborCount = 7;
    myCells[19].neighbors = (uint16_t *)malloc(myCells[19].neighborCount*sizeof(uint16_t));
    myCells[19].neighbors[0] = 3;
    myCells[19].neighbors[1] = 8;
    myCells[19].neighbors[2] = 9;
    myCells[19].neighbors[3] = 13;
    myCells[19].neighbors[4] = 14;
    myCells[19].neighbors[5] = 18;
    myCells[19].neighbors[6] = 20;

    myCells[20].pin = PD5;
    myCells[20].neighborCount = 7;
    myCells[20].neighbors = (uint16_t *)malloc(myCells[20].neighborCount*sizeof(uint16_t));
    myCells[20].neighbors[0] = 4;
    myCells[20].neighbors[1] = 9;
    myCells[20].neighbors[2] = 10;
    myCells[20].neighbors[3] = 14;
    myCells[20].neighbors[4] = 15;
    myCells[20].neighbors[5] = 16;
    myCells[20].neighbors[6] = 19;

    return myCells;
}

void setCellState(struct cell *myCell, uint8_t state) {
    myCell->state = state;
}
