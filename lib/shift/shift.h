#ifndef SHIFT_H
#define SHIFT_H

#include <avr/io.h> 
#include <util/delay.h>
#include <stdint.h>
#include "../state/state.h"

// update if circuit has a different number of shift registers
#define NUM_SR 4
#define BITS (NUM_SR * 8)

// digital pins for shift register
#define SER (1 << PD2) // D2
#define OE (1 << PD3) // D3
#define LATCH (1 << PD4) // D4
#define CLOCK (1 << PD5) // D5

// struct to structure shift register pins together
typedef struct {
    uint8_t ser;
    uint8_t oe;
    uint8_t latch;
    uint8_t clock;
} shiftReg;

void shift_init(shiftReg *sr); // setup shift register struct
void delay_ms_var(uint8_t ms); // variable delay time
void set_brt(); // set shift register LED brightness
void del(); // calls delay_ms_var with variable delay from speed pot 
void pulse_pin(shiftReg *sr, uint8_t clk_latch); // set/clear clock or latch bit
// void byte_chaser(shiftReg *sr, state *st, int num_sr, uint8_t rev, uint8_t lcd); // byte
// void bit_chaser(shiftReg *sr, state *st, uint8_t num_sr, uint8_t rev);
#endif