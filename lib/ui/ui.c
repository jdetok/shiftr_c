#include "ui.h"

// ui.c
// INITALIZE UI COMPONENTS

// setup digital pins (& analog A0 A5) for switches
void switch_init(switches *sw) {
    *sw = (switches){
        .switches = {
            {.pin = PWR_PIN, .bit = PWR_BIT, .reg = 'b'},
            // {.pin = SEQ_PIN, .bit = SEQ_BIT, .reg = 'b'},
            // {.pin = REV_PIN, .bit = REV_BIT, .reg = 'b'},
            // {.pin = MOD_PIN, .bit = MOD_BIT, .reg = 'b'},
            // {.pin = RGB_PIN, .bit = RGB_BIT, .reg = 'b'},
        },
        .state = 0
    };
}

// setup analog pins
void pot_init() {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// control sr oe pin (all leds brightness) with OCR0A
void oe_pwm() {
    // Fast PWM, non-inverting, 8-bit
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
    TCCR2B = (1 << CS21);  // prescaler = 8
}
