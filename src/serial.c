/*
	A minimal example of serial communication for the ATMega328P.
*/

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

void serial_init(void) {
	// Set baud rate
	UBRR0H = (unsigned char) (MYUBBR >> 8);
	UBRR0L = (unsigned char) MYUBBR;
	// Set framing to 8N1
	UCSR0C = ( 3 << UCSZ00 );
	// Enable RX and TX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	return;
}

void serial_write(char c) {
	while ( !(UCSR0A & (1 << UDRE0)) ) {
	}

	UDR0 = c;
}
