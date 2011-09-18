#include <avr/io.h>
#include <util/delay.h>

#define MHZ 8

// Number of pins avaiable through serial->parallel
// shift registers (ie: 74HC595)
#define SHIFT_OUT_PIN_COUNT 24

// Serial->Parallel for LEDs
#define RCLK    PB0
#define SER     PB1
#define SRCLK   PB2

// Parallel->Serial for Rules DIP switches
#define RULES_CP    PD2
#define RULES_PS    PD3
#define RULES_DO    PD4

// Parallel->Serial for Initial State DIP switches
#define INIT_CP     PD5
#define INIT_PS     PD6
#define INIT_DO     PD7

void delay_ms(uint16_t x);
void led_shift_init(void);
void rules_shift_init(void);
uint16_t read_rules(void);
void update_leds(uint32_t new_states, uint8_t pins_to_shift);


int main(void){
    led_shift_init();
    rules_shift_init();

    while (1) {

        uint16_t rules = read_rules();
        update_leds(rules,24);

    }

    return 0;
}

void rules_shift_init(void) {
    // Latch and Clock set to output
    DDRD |= _BV(RULES_PS) | _BV(RULES_CP);
    // Data In as input
    DDRD &= ~_BV(RULES_DO);
}

void led_shift_init(void) {
    // 
    DDRB |= _BV(RCLK) | _BV(SRCLK) | _BV(SER);
}

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
    This fuction reads the status of the dip switches
    used for rules by shifting in their values with a 4021
    shift register. Returns the rules as a 16 bit value each
    bit of which represents a switch state. bit 0 is switch 1.
*/
uint16_t read_rules() {
    uint16_t rules = 0;

    PORTD |= _BV(RULES_PS);
    _delay_us(20);
    PORTD &= ~_BV(RULES_PS);

    int i;
    for (i = 0; i < 16; i++) {
        _delay_us(2);

        if ( PIND & _BV(RULES_DO) )
            rules |= (uint16_t)1 << i;
        _delay_us(20);
        PORTD &= ~_BV(RULES_CP);
        PORTD |= _BV(RULES_CP);
    }
    return rules;
} 

// shift a number out.  pins_to_shift is how many bits to shift
// buffering with zero instead of the new_states' bit value
void update_leds(uint32_t new_states, uint8_t pins_to_shift) {
    PORTB &= ~_BV(RCLK);

    for (uint8_t i = 0; i < SHIFT_OUT_PIN_COUNT; i++) {
        PORTB &= ~_BV(SRCLK);
        if (pins_to_shift && new_states & ((uint32_t)1 << i)) {
            PORTB |= _BV(SER);
            pins_to_shift--;
        }
        PORTB |= _BV(SRCLK);
        PORTB &= ~_BV(SER);
    }
    PORTB |= _BV(RCLK);

}
