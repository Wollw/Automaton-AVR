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
#define CELL_COUNT 21

// ioinit and delay_ms taken from a SparkFun example by Nathan Seidle
// Define functions
void ioinit(void);            //Initializes IO
void delay_ms(uint16_t x); //General purpose delay
struct cell *createCells(uint8_t cellCount); // create cellCount cells
void setCellState(struct cell *myCell,  // Change the cell state to 'state'
                  uint8_t state);       // 0 = off, anything else = on
void displayBoard(struct cell *myCells); // Display the state of the board with LEDs on IO
void blinkLEDs(uint16_t ms);    // Blink LEDs for a total of 'ms' milliseconds;
void updateCellStates(struct cell *myCells); // Apply the rules of the game to the cells

// Each instance of a cell holds current state, an id, and the neihbours
struct cell {
    unsigned neighborCount :5;
    unsigned state :1;
    unsigned stateNext :1;
    uint8_t pin;
    uint32_t neighbors; // neighbors stored as bits to save ram
};

int main (void)
{
    ioinit(); //Setup IO pins and defaults

    blinkLEDs(1000);
    delay_ms(1000);

    struct cell *cells = createCells(CELL_COUNT);

    // Start these cells off living
    cells[19].state = ON;

    // Display initial state of board with LEDs

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
    DDRB = 0b11111111; //All outputs
    DDRC = 0b11111111; //All outputs
    DDRD = 0b11111111; //All outputs
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
*   cellCount : number of cells to reserve space in sram for 
*/
struct cell* createCells(uint8_t cellCount) {
    struct cell *myCells;
    myCells = (struct cell *)malloc(cellCount*sizeof(struct cell));

    myCells[0].pin = PB0;
    myCells[0].neighborCount = 10;
    myCells[0].neighbors = 0b11111111110;

    myCells[1].pin = PB1;
    myCells[1].neighborCount = 8;
    myCells[1].neighbors = 0b100001100011100101;

    myCells[2].pin = PB2;
    myCells[2].neighborCount = 8;
    myCells[2].neighbors = 0b1000011000110001011;

    myCells[3].pin = PB3;
    myCells[3].neighborCount = 8;
    myCells[3].neighbors = 0b10000110001100010101;

    myCells[4].pin = PB4;
    myCells[4].neighborCount = 8;
    myCells[4].neighbors = 0b100001100011000101001;

    myCells[5].pin = PB5;
    myCells[5].neighborCount = 8;
    myCells[5].neighbors = 0b11000110001010011;

    myCells[6].pin = PB6;
    myCells[6].neighborCount = 10;
    myCells[6].neighbors = 0b111001110010100011;

    myCells[7].pin = PB7;
    myCells[7].neighborCount = 10;
    myCells[7].neighbors = 0b1100011100101000111;

    myCells[8].pin = PC0;
    myCells[8].neighborCount = 10;
    myCells[8].neighbors = 0b11000111001010001101;

    myCells[9].pin = PC1;
    myCells[9].neighborCount = 10;
    myCells[9].neighbors = 0b110001110010100011001;

    myCells[10].pin = PC2;
    myCells[10].neighborCount = 10;
    myCells[10].neighbors = 0b100011100101001110001;

    myCells[11].pin = PC3;
    myCells[11].neighborCount = 9;
    myCells[11].neighbors = 0b111001010011100010;

    myCells[12].pin = PC4;
    myCells[12].neighborCount = 9;
    myCells[12].neighbors = 0b1100010100111000110;

    myCells[13].pin = PC5;
    myCells[13].neighborCount = 9;
    myCells[13].neighbors = 0b11000101001110001100;

    // Skipping PC6 as it is the reset pin

    myCells[14].pin = PD0;
    myCells[14].neighborCount = 9;
    myCells[14].neighbors = 0b110001010011100011000;

    myCells[15].pin = PD1;
    myCells[15].neighborCount = 9;
    myCells[15].neighbors = 0b100010100111001110000;

    myCells[16].pin = PD2;
    myCells[16].neighborCount = 7;
    myCells[16].neighbors = 0b100101000110001100000;

    myCells[17].pin = PD3;
    myCells[17].neighborCount = 7;
    myCells[17].neighbors = 0b1010001100011000010;

    myCells[18].pin = PD4;
    myCells[18].neighborCount = 7;
    myCells[18].neighbors = 0b10100011000110000100;

    myCells[19].pin = PD5;
    myCells[19].neighborCount = 7;
    myCells[19].neighbors = 0b101000110001100001000;

    myCells[20].pin = PD6;
    myCells[20].neighborCount = 7;
    myCells[20].neighbors = 0b10011100011000010000;

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
            case 0:
                if (i < 8)
                    PORTB &= ~_BV(myCells[i].pin);
                else if (i <  14)
                    PORTC &= ~_BV(myCells[i].pin);
                else 
                    PORTD &= ~_BV(myCells[i].pin);
                break;
            case 1:
                if (i < 8)
                    PORTB |= _BV(myCells[i].pin);
                else if (i <  14)
                    PORTC |= _BV(myCells[i].pin);
                else
                    PORTD |= _BV(myCells[i].pin);
                break;
        }
    }
}

// Blink LEDs on and off for a total of 'ms' milliseconds
void blinkLEDs(uint16_t ms) {
    PORTB = _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) |
             _BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7);
    PORTC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3) |
             _BV(PC4) | _BV(PC5);
    PORTD = _BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3) |
             _BV(PD4) | _BV(PD5) | _BV(PD6);
    delay_ms(ms/2);
    PORTB = ~(_BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) |
               _BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7));
    PORTC = ~(_BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3) |
               _BV(PC4) | _BV(PC5));
    PORTD = ~(_BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3) |
               _BV(PD4) | _BV(PD5) | _BV(PD6));
    delay_ms(ms/2);
}

// Apply the rules of the game to the cells
void updateCellStates(struct cell *myCells) {
    uint8_t i;
    for (i = 0; i < CELL_COUNT; i++) {
        uint8_t j;
        int n = 0;
        for (j = 0; j < CELL_COUNT; j++) {
            if ( (1 << j) & myCells[i].neighbors )
                n += myCells[j].state;
        }

        // Survival Rules
        if (myCells[i].state == ON) {
            switch (n) {
                case 0:
                case 4:
                case 5:
                    myCells[i].stateNext = ON;
                    break;
            }
        }
        // Birth Rules
        else if (myCells[i].state == OFF) {
            switch (n) {
                case 0:
                case 5:
                case 6:
                    myCells[i].stateNext = ON;
                    break;
            }
        }
        else {
            myCells[i].stateNext = OFF;
        }
    }
    // Update state
    for (i = 0; i < CELL_COUNT; i++) {
        myCells[i].state = myCells[i].stateNext;
    }
}
