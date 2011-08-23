#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
//Define functions
//======================
void ioinit(void);			//Initializes IO
void delay_ms(uint16_t x); //General purpose delay
//======================

// Each instance of a cell holds current state, an id, and the neihbours
struct cell {
	unsigned short neighbourCount;
	unsigned state :1;
	unsigned stateNext :1;
	unsigned short pin;
	unsigned short *neighbours;
};

int main (void)
{
		ioinit(); //Setup IO pins and defaults

	struct cell cells[9];

	int i;
	for (i = 0; i < 9; i++) {
		cells[i].state = 0;
		cells[i].neighbours = (unsigned short *)
								malloc(8*sizeof(unsigned short));
		memset(cells[i].neighbours, -1, 8*sizeof(unsigned short));
	}

	cells[0].pin = PB1;
	cells[0].neighbours[0] = 3;
	cells[0].neighbours[1] = 4;
	cells[0].neighbours[2] = 1;
	cells[0].neighbourCount = 3;

	cells[1].pin = PB2;
	cells[1].neighbours[0] = 3;
	cells[1].neighbours[1] = 4;
	cells[1].neighbours[2] = 5;
	cells[1].neighbours[3] = 0;
	cells[1].neighbours[4] = 2;
	cells[1].neighbourCount = 5;

	cells[2].pin = PB3;
	cells[2].neighbours[0] = 4;
	cells[2].neighbours[1] = 5;
	cells[2].neighbours[2] = 1;
	cells[2].neighbourCount = 3;

	cells[3].pin = PB4;
	cells[3].neighbours[0] = 4;
	cells[3].neighbours[1] = 0;
	cells[3].neighbours[2] = 1;
	cells[3].neighbours[3] = 6;
	cells[3].neighbours[4] = 7;
	cells[3].neighbourCount = 5;

	cells[4].pin = PB5;
	cells[4].neighbours[0] = 3;
	cells[4].neighbours[1] = 5;
	cells[4].neighbours[2] = 0;
	cells[4].neighbours[3] = 1;
	cells[4].neighbours[4] = 2;
	cells[4].neighbours[5] = 6;
	cells[4].neighbours[6] = 7;
	cells[4].neighbours[7] = 8;
	cells[4].neighbourCount = 8;

	cells[5].pin = PC0;
	cells[5].neighbours[0] = 4;
	cells[5].neighbours[1] = 1;
	cells[5].neighbours[2] = 2;
	cells[5].neighbours[3] = 7;
	cells[5].neighbours[4] = 8;
	cells[5].neighbourCount = 5;

	cells[6].pin = PC1;
	cells[6].neighbours[0] = 3;
	cells[6].neighbours[1] = 4;
	cells[6].neighbours[2] = 7;
	cells[6].neighbourCount = 3;

	cells[7].pin = PC2;
	cells[7].neighbours[0] = 3;
	cells[7].neighbours[1] = 4;
	cells[7].neighbours[2] = 5;
	cells[7].neighbours[3] = 6;
	cells[7].neighbours[4] = 8;
	cells[7].neighbourCount = 5;

	cells[8].pin = PC3;
	cells[8].neighbours[0] = 4;
	cells[8].neighbours[1] = 5;
	cells[8].neighbours[2] = 7;
	cells[8].neighbourCount = 3;

	cells[3].state = 1;
	cells[4].state = 1;
	cells[5].state = 1;


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
		for (i = 0; i < 9; i++) {
			int j;
			int n = 0;
			for (j = 0; j < cells[i].neighbourCount; j++) {
				n += cells[cells[i].neighbours[j]].state;
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
		for (i = 0; i < 9; i++) {
			cells[i].state = cells[i].stateNext;
		}

		// Display new state
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
	}

	// Free the allocated memory even though we should never get here
	for (i = 0; i < 9; i++) {
		free(cells[i].neighbours);
		cells[i].neighbours = NULL;
	}
	 
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
