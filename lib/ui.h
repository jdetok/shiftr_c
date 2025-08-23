#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

typedef struct {
    uint8_t d9; // power on off
    uint8_t d10; // d10uence or solid
    uint8_t d11; // intensity mode (only in chaser)    
    uint8_t d12; // chaser d12erse
    uint8_t d13; // d13 on off
} sw_states;

typedef struct {
    uint8_t d9_sw; // d5 - on off
    uint8_t d10_sw; // d3 - solid on or d10uence
    uint8_t d11_sw; // d2 - intensity    
    uint8_t d12_sw; // a0 (a7)
    uint8_t d13_sw; // a5 (a7)
    sw_states states;
} switches;

void pot_init();
void switch_init(switches *sw);
void current_states(switches *sw);
uint8_t get_state(uint8_t pin);
uint8_t sum_states(switches *sw);
uint8_t update_states(switches *sw);
uint16_t read_pot(uint8_t channel);
#endif