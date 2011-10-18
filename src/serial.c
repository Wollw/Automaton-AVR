/*	Automaton AVR - serial.c
 *
 *	(c) 2011 - David Ellis Shere
 *
 *	'serial_init' sets up the pins for serial communication.
 *
 *	'serial_write' writes a char to serial output
*/

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

/*
 *	setup the serial hardware for 9600bps
*/
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

/*
 * write a char to the serial port.
*/
void serial_write(char c) {
	while ( !(UCSR0A & (1 << UDRE0)) ) {
	}

	UDR0 = c;
}
