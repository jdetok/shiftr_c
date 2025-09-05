#include "state.h"

/* TODO:
create a single struct that stores buttons state and switch state
- since there's only one switch, i should just manually read that pin
*/

void btns_init() {
    DDRB |= CE_PIN | CLK_PIN | PL_PIN; // input sr output pins
    DDRB &= ~DATA_PIN; // data in as input
}

void state_init(state *st) {
    // set btn_now and btn_state as reading
    st->btn_now = read_btns();
    st->btn_state = st->btn_now;
}

uint8_t read_btns() {
    uint8_t val = 0;

    // pl pulse, parallel inputs into shift register
    // note, removed a delay
    PORTB &= ~PL_PIN;
    PORTB |= PL_PIN;

    // clock enable low (active)
    PORTB &= ~CE_PIN; 

    for (uint8_t i = 0; i < 8; i++) {
        val <<= 1; // left shift 1

        if (PINB & DATA_PIN) {
            val |= 1; // shift in a 1 to most sig bit (rightmost)
        }
        
        // pulse clock
        PORTB |= CLK_PIN;
        PORTB &= ~CLK_PIN;    
    }
    PORTB |= CE_PIN; // disable clock

    return ~val;
}

// read state, check if it's different than retained state
uint8_t state_changed(state *st) {
    // read current buttons state
    uint8_t prev = st->btn_now;
    st->btn_now = read_btns();

    // xor to detect changes
    uint8_t rising = (~prev) & st->btn_now;

    // flip changed bits
    if (rising) {
        for (uint8_t i = B0; i < BTN_CNT; i++) {    
            if (rising & (1 << i)) {
                st->btn_state ^= (1 << i);
            }
        }
        return 1;
        }
    return 0;
}