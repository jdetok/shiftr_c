#include "ui.h"

// setup digital pins d9-d13
void switch_init(switches *sw) {
    sw->d9_sw = 1 << PB1;
    sw->d10_sw = 1 << PB2;
    sw->d11_sw = 1 << PB3;
    sw->d12_sw = 1 << PB4;
    sw->d13_sw = 1 << PB5;
    DDRB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));

}

// setup analog pins
void pot_init() {
    // Reference = AVcc (5V) with external capacitor at AREF
    ADMUX = (1 << REFS0);

    // Enable ADC, prescaler = 128 -> ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// read analog channel
uint16_t read_pot(uint8_t channel) {
    // select channel (0–7), clear MUX bits first
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // start conversion
    ADCSRA |= (1 << ADSC);

    // wait for conversion to finish
    while (ADCSRA & (1 << ADSC));

    // don't return 255, will turn all LED off
    uint16_t val = ADC >> 2;
    if (val > 254) {
        return 254;
    }
    return val;  // 10-bit result (0–1023)
}

uint8_t get_state(uint8_t pin) {
    // return PINB & pin;
    return (PINB & pin) ? 1 : 0;
//    if (reg == 'd') {
//         return PIND & pin;    
//    }
//    if (reg == 'c') {
//     return PINC & pin;
//    }
}

// TODO: new get_states() that accepts a pointer to a states struct & updates it
// 1770 bytes before implementing

void current_states(switches *sw) {
    sw->states.d9 = get_state(sw->d9_sw);
    sw->states.d10 = get_state(sw->d10_sw);
    sw->states.d11 = get_state(sw->d11_sw);
    sw->states.d12 = get_state(sw->d12_sw);
    sw->states.d13 = get_state(sw->d13_sw);
}

// sum the states, use in update_states to determine whether there was a state change
uint8_t sum_states(switches *sw) {
    return (
        sw->states.d9 + sw->states.d10 + sw->states.d11 + sw->states.d12 + sw->states.d13);
}

uint8_t update_states(switches *sw) {
    // record state before update
    uint8_t stSum = sum_states(sw);

    // get current states
    current_states(sw);

    // check if state sum if different after update, return 1 if so
    if (stSum != sum_states(sw)) {
        return 1;
    }
    return 0;
}