#include <util/delay.h>

void delay_ms(uint16_t count) {
	while (count--) {
		_delay_ms(1);
	}
}

/*
*	Delay an amount determined by how far between 0 and 1023 value is
*/
void delay_ms_scaled(uint16_t min, uint16_t max, uint16_t value) {
	uint16_t delay = (int)(((float)value/1023) * max) + min;
	delay_ms(delay);
}
