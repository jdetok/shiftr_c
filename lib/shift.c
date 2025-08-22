#include "shift.h"

// required to accept delay time as a variable from pot
// _delay_ms() only accepts a compile time constant
void delay_ms_var(uint8_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

// delay
void del() {
    delay_ms_var(read_pot(1)); // delaytime ms
}

void shift_init(shiftReg *sr) {
    // set pins to the appropriate bit on the register
    sr->ser = 1 << PD2;
    sr->oe = 1 << PD3;
    sr->latch = 1 << PD4;
    sr->clock = 1 << PD5;

    // setup pins as output on their appropriate register
    DDRD |= sr->ser | sr->oe | sr->latch | sr->clock;
    

    oe_pwm(); // setup pwm channel for oe pin to control brightness
}

// read intensity switch, return value will be num_sr value in chaser
uint8_t read_intn(uint8_t channel) {
    uint16_t val = read_pot(channel);
    if (val < 50) {
        return 1;
    } else if (val < 150) {
        return 2;
    } else {
        return 3;
    }
}

// control sr oe pin (all leds brightness) with OCR0A
void oe_pwm() {
    // Fast PWM, non-inverting, 8-bit
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
    TCCR0B = (1 << CS21);  // prescaler = 8
}

// set brightness for leds as analog reading of pin A1 
void set_brt() {
    OCR2B = read_pot(0); // set brightness
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

// turn off all lights
void onoff(shiftReg *sr, switches *sw, int num_sr, int on) {
    int bits = num_sr * 8;
    for (int i = 0; i < bits; i++) {
        uint8_t interrupt = update_states(sw);
        if (interrupt) {
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

// TODO: new chaser accept readA0 output as param forward reverse
void chaser(shiftReg *sr, switches *sw, int num_sr, uint8_t rev) {
    int bits = num_sr * 8;
    if (!rev) {
        // outer loop through number of LEDs
        for (int i = 0; i < bits; i++) {
            // check that switch states haven't changed, exit if it has
            // uint8_t interrupt = update_stastes(sw);
            // if (interrupt) {
            //     return;
            // } else {
            //     set_brt(); // set brightness
                // loop forward
                for (int b = (bits - 1); b >= 0; b--) {
                    if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                        PORTD |= sr->ser; // write a 1
                    } else {
                        PORTD &= ~sr->ser; // write a 0
                    }
                    pulse_pin(sr, 0); // shift clock
                // }
            }
            pulse_pin(sr, 1); // shift latchs
//            del(); // delay
            delay_ms_var(200);

        }
    } else { // REVERSE
        for (int i = bits; i >= 0; i--) {
        // read current states & return if change detected
            // uint8_t interrupt = update_states(sw);
            // if (interrupt) {
            //     return;
            // } else {
            //     set_brt(); // set brightness
                for (int b = (bits - 1); b > 0; b--) {
                    if (b == i) { // when current bit position (b) is same as current led (i), send a 1 to serial pin
                        PORTD |= sr->ser; // write a 1
                    } else {
                        PORTD &= ~sr->ser; // write a 0
                    }
                    pulse_pin(sr, 0); // pulse clock
                }
            // }
            pulse_pin(sr, 1); // pulse latch
            delay_ms_var(200);
            // del(); // delay
        }
    }
}

void byte_chaser(shiftReg *sr, switches *sw, int num_sr) {
    uint8_t bits = num_sr * 8;
    // outer loop through number of shift registers
    for (int i = 0; i < num_sr; i++) {
        // check that switch states haven't changed, exit if it has
        uint8_t interrupt = update_states(sw);
        if (interrupt) {
            return;
        } else {
            set_brt(); // set brightness
            // loop through 56 bits, use i to figure out which 8 to light
            for (int b = bits - 1; b >= 0; b--) {
                if (b / 8 == i) {
                    PORTD |= sr->ser; // write a 1
                } else {
                    PORTD &= ~sr->ser; // write a 0
                }
                pulse_pin(sr, 0); // shift clock
            }
        }
        pulse_pin(sr, 1); // shift latchs
        del(); // delay
    }
}