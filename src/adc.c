#include "adc.h"

void adc_init(uint8_t prescaler) {
	// Enable ADC and set prescaler
	ADCSRA |= _BV(ADEN) | prescaler;
	return;
}

int adc_read(uint8_t adcx) {
	// Set ADC number 
	ADMUX	|=	adcx;

	
	// Do conversion, wait for result
	ADCSRA |= _BV(ADSC);
	while ( (ADCSRA & _BV(ADSC)) );

	return ADC;
}
