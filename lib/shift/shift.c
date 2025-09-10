#include "shift.h"

// required to accept delay time as a variable from pot
// _delay_ms() only accepts a compile time constant
void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_us(800);
    }
}

// delay
void del() {
    delay_ms_var(read_pot(SPD_POT)); // delaytime ms
}

// set brightness for leds as analog reading of pin A1 
void set_brt() {
    OCR2B = read_pot(BRT_POT); // set brightness
}

void shift_init(shiftReg *sr) {
    // set pins to the appropriate bit on the register
    sr->ser = SER;
    sr->oe = OE;
    sr->latch = LATCH;
    sr->clock = CLOCK;

    // setup pins as output on their appropriate register
    DDRD |= sr->ser | sr->oe | sr->latch | sr->clock;
    
    oe_pwm(); // setup pwm channel for oe pin to control brightness
}

// 0 for clock, 1 for latch
void pulse_pin(shiftReg *sr, uint8_t clk_latch) {
    switch (clk_latch) {
        case 0: // clock pin
            PORTD |= sr->clock;
            PORTD &= ~sr->clock;
            break;
        case 1: // latch pin
            PORTD |= sr->latch;
            PORTD &= ~sr->latch;
            break;
    }
}
