/*
	ADC reading and initializing
*/
#include <avr/io.h>
#include "config.h"

#define	ADC_PIN			3
#define ADC_PRESCALE	0b011	// 0b011 is prescale by 8

void adc_init(uint8_t prescaler);
int adc_read(uint8_t adcx);
