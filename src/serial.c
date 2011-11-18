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
#include "config.h"
#include "serial.h"

/*
 *	setup the serial hardware
*/
void serial_init(void) {
	// Set baud rate
	UBRR0H = (unsigned char) (MYUBBR >> 8);
	UBRR0L = (unsigned char) MYUBBR;

	// Set framing to 8N1
	UCSR0C = ( 3 << UCSZ00 );

	// Enable RX and TX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

/*
 * write a char to the serial port.
*/
void serial_write(char c) {
	// Wait until serial is ready to output char
	while (!(UCSR0A & (_BV(UDRE0))));
	UDR0 = c;
}

/*
 *	output bits of a uint32_t
*/
void serial_write_bits_u32(uint32_t bits) {
	uint32_t bit_count = 32;
	for (uint8_t i = 0; i < bit_count; i++) {
		if (bits & ((uint32_t)1 << i))
			serial_write('1');
		else
			serial_write('0');
	}
	serial_write('\n');	
	serial_write('\r');
}
