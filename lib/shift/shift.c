#include "shift.h"
/*
void mode_selector(shiftReg *sr, switches *sw, uint8_t mode, uint8_t lcd) {
    switch (mode) {
    case 1:
        chaser(sr, sw, 2, switch_state(sw, REV_SW), lcd);
        break;
    case 2:
        chaser(sr, sw, 3, switch_state(sw, REV_SW), lcd);
        break;
    case 3:
        byte_chaser(sr, sw, NUM_SR, switch_state(sw, REV_SW), lcd);        
        break;
    }
    
}
*/
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
    // DDRB |= ; 

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

// turn off all lights // lcd 0 or 1 to enable prints
void onoff(shiftReg *sr, state *st, int num_sr, int on, uint8_t lcd) {
    int bits = num_sr * 8;
    for (int i = 0; i < bits; i++) {
        // uint8_t interrupt = update_states(sw);
        if (state_changed(st)) {
            return;
        } else {
            set_brt();
            if (on) {
                PORTD |= sr->ser;
            } else {
                PORTD &= ~sr->ser;
            }
            pulse_pin(sr, 0); // pulse clock
        }
    }
    pulse_pin(sr, 1); // pulse latch
}

// Chaser without 64-bit math
void bit_chaser(shiftReg *sr, state *st, uint8_t num_sr) {
    uint8_t total_bits = num_sr * 8;

    for (uint8_t pos = 0; pos < total_bits; pos++) {
        if (state_changed(st)) {
            return; // abort if state changed
        }

        // Send out all bits for all registers
        for (int8_t b = total_bits - 1; b >= 0; b--) {
            if (b == pos) {
                PORTD |= sr->ser;   // set SER high at current chaser bit
            } else {
                PORTD &= ~sr->ser;  // otherwise low
            }
            pulse_pin(sr, 0);       // clock the bit into the chain
        }

        pulse_pin(sr, 1);   // latch output
        del();              // delay for visible chaser speed
    }
}



void byte_chaser(shiftReg *sr, state *st, int num_sr, uint8_t rev, uint8_t lcd) {
    uint8_t bits = num_sr * 8;
    uint64_t bitmask = rev ? (0xFFULL << (bits - 8)) : 0xFFULL;
    
    // outer loop through number of shift registers
    for (int i = 0; i < num_sr; i++) {
        // check that switch states haven't changed, exit if it has
        if (state_changed(st)) {
            return; // return if state changed
        }
        set_brt(); // set brightness
        uint64_t temp = bitmask;
        for (int b = 0; b < bits; b++) {
            if (temp & 1ULL) {
                PORTD |= sr->ser;  // write 1
            } else {
                PORTD &= ~sr->ser; // write 0
            }
            pulse_pin(sr, 0);      // shift clock
            temp >>= 1;             // move next bit into LSB
        }
        pulse_pin(sr, 1); // shift latchs
        del(); // delay
        bitmask = rev ? (bitmask >> 8ULL) : (bitmask << 8ULL);
    }
}