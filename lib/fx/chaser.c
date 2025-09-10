#include "fx.h"

void bit_chaser(shiftReg *sr, state *st, uint8_t num_sr, uint8_t rev) {
    uint8_t total_bits = num_sr * 8;

    for (uint8_t step = 0; step < total_bits; step++) {
        if (state_changed(st)) {
            return; // abort if state changed
        }

        uint8_t pos = rev ? (total_bits - 1 - step) : step;
        for (int8_t b = total_bits - 1; b >= 0; b--) {
            set_brt();
            if (b == pos) {
                PORTD |= sr->ser;
            } else {
                PORTD &= ~sr->ser;
            }
            pulse_pin(sr, 0);
        }

        pulse_pin(sr, 1);
        del();
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