// Each instance of a cell holds current state, an id, and the neihbours
struct cellStruct {
    unsigned state :1;
    unsigned stateNext :1;
    uint8_t *port;
    uint8_t pin;
    uint32_t neighbors; // neighbors stored as bits to save ram
};

// The rules are stored as bits offset by the living neighbor count
// ie : if there a rule set is 0b0100 for cell birth and a cell has 2 neighbors
// and is currently dead it will come to life
struct ruleStruct {
    uint32_t survival;
    uint32_t birth;
};

// ioinit and delay_ms taken from a SparkFun example by Nathan Seidle
// Define functions
void ioinit(void);            //Initializes IO
void delay_ms(uint16_t x); //General purpose delay
struct cellStruct *createCells(uint32_t initialState); // create CELL_COUNT cells
void setCellState(struct cellStruct *myCell,  // Change the cell state to 'state'
                  uint8_t state);       // 0 = off, anything else = on
void displayBoard(struct cellStruct *myCells); // Display the state of the board with LEDs on IO
void blinkLEDs(uint16_t ms);    // Blink LEDs for a total of 'ms' milliseconds;
void applyRules(struct cellStruct *myCells, struct ruleStruct *myRules); // Apply the rules of the game to the cells
uint8_t getCellCount(struct cellStruct *myCells, uint8_t state);
