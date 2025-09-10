#include "state.h"


// setup analog pins
void pot_init() {
    // Reference = AVcc (5V) with external capacitor at AREF
    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// control sr oe pin (all leds brightness) with OCR0A
void oe_pwm() {
    // Fast PWM, non-inverting, 8-bit
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
    TCCR2B = (1 << CS21);  // prescaler = 8
}

// read analog channel
uint8_t read_pot(uint8_t channel) {
    
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // select chnl (0–7), clear mux
    ADCSRA |= (1 << ADSC); // start conversion

    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish

    // don't return 255, will turn all LED off
    uint16_t val = ADC >> 2;
    if (val > 254) {
        return 254;
    // } else if (val <= 20) {
    //     return 0;
    }
    return val;
}