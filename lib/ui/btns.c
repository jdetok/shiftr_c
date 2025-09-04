#include "ui.h"

static void hc165_pulse(volatile uint8_t *port, uint8_t pin) {
    *port |=  pin;
    *port &= ~pin;
}

void btns_init(buttons *btns) {
    *btns = (buttons){
        .btn = {
            {.sr_pos = (1 << SH0), .state = 0, .name = "t1"},
            {.sr_pos = (1 << SH1), .state = 0, .name = "t2"},
            {.sr_pos = (1 << SH2), .state = 0, .name = "t3"},
            {.sr_pos = (1 << SH3), .state = 0, .name = "t4"},
            {.sr_pos = (1 << SH4), .state = 0, .name = "t5"},
            {.sr_pos = (1 << SH5), .state = 0, .name = "t6"},
            {.sr_pos = (1 << SH6), .state = 0, .name = "t7"},
            {.sr_pos = (1 << SH7), .state = 0, .name = "t8"}
        }, 
        .state = 0
    };

    DDRB |= CE_PIN | CLK_PIN | PL_PIN; // input sr output pins
    DDRB &= ~DATA_PIN; // data in as input
}

void check_btns(buttons *btns) {
    uint8_t state = hc165_read();

    if (state != btns->state) {
        char binval[9];
        for (int b = 7; b >= 0; b--) {
            binval[7 - b] = (state & (1 << b)) ? '1' : '0';
        }
        binval[8] = '\0';
        lcd_goto_print(0, 0, binval);

        for (uint8_t i = SH0; i < BTN_CNT; i++) {    
            if (state & (btns->btn[i].sr_pos) && !(btns->state & (btns->btn[i].sr_pos))) {
                btns->btn[i].state ^= 1;
                lcd_goto_print(1, 0, btns->btn[i].name);
                lcd_goto_print(1, 3, "state: ");
                char buf[2];
                itoa(btns->btn[i].state, buf, 10);
                lcd_goto_print(1, 9, buf);
            }
        }
        btns->state = state;
    }
}


uint8_t hc165_read(void) {
    uint8_t val = 0;

    // pl pulse, parallel inputs into shift register
    PORTB &= ~PL_PIN;
    _delay_us(1);
    PORTB |= PL_PIN;

    PORTB &= ~CE_PIN; // clock enable low (active)

    for (uint8_t i = 0; i < 8; i++) {
        val <<= 1; // left shift 1

        if (PINB & DATA_PIN) {
            val |= 1; // shift in a 1 to most sig bit (rightmost)
        }
        
        // pulse clock
        PORTB |= CLK_PIN;
        _delay_us(1);
        PORTB &= ~CLK_PIN;    
    }
    PORTB |= CE_PIN; // disable clock

    return ~val;
}