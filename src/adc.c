#include "adc.h"
#include <avr/io.h>

void adc_init(uint8_t prescaler) {
	// Enable ADC and set prescaler
	ADCSRA |= _BV(ADEN) | prescaler;
	return;
}

int adc_read(uint8_t adcx) {
	// Set ADC number 
	ADMUX	|=	adcx;

	// Use AVCC for voltage reference on ATmega328P
	#ifdef __AVR_ATmega328P__
	ADMUX	|=	_BV(REFS0);
	#endif

	// Do conversion, wait for result
	ADCSRA |= _BV(ADSC);
	while ( (ADCSRA & _BV(ADSC)) );

	return ADC;
}
