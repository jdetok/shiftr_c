#include "state.h"

void btns_init(state *st) {
    // set pins as input/output
    DDRB |= CE_PIN | CLK_PIN | PL_PIN; // input sr output pins
    DDRB &= ~DATA_PIN; // data in as input
    
    // init state
    st->btn_now = read_btns();
    st->btn_state = st->btn_now;
}

// pass a single button (use BTN* macros), whether its bit is set
uint8_t ison(state *st, uint8_t btn) {
    return (st->btn_state & btn);
}

// print out full 8 bit button state
void print_state(state *st) {
    char binval[9];
    for (int b = 7; b >= 0; b--) {
        binval[7 - b] = (st->btn_state & (1 << b)) ? '1' : '0';
    }
    binval[8] = '\0';
    lcd_goto_print(1, 0, binval);
}

uint8_t read_btns() {
    uint8_t val = 0;

    // pl pulse, parallel inputs into shift register
    PORTB &= ~PL_PIN;
    PORTB |= PL_PIN;

    // clock enable low (active)
    PORTB &= ~CE_PIN; 

    for (uint8_t i = 0; i < 8; i++) {
        val <<= 1; // left shift 1

        // if bit is set, button is not pressed
        if (PINB & DATA_PIN) {
            val |= 1; // shift in a 1 to most sig bit (rightmost)
        }
        
        // pulse clock
        PORTB |= CLK_PIN;
        PORTB &= ~CLK_PIN;    
    }
    PORTB |= CE_PIN; // disable clock

    // return inverse since buttons are pulled to ground
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