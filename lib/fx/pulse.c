#include "fx.h"

// all LEDs on, pulse brightness from 0 -> brightness pot value
void pulse(shiftReg *sr, state *st) {
    uint8_t off = 255;
    // uint8_t val;
    // uint8_t max_brt = read_pot(BRT_POT);

    for (uint8_t i = 0; i < BITS; i++) {
        if (state_changed(st)) return;
        PORTD |= sr->ser;
        pulse_pin(sr, 0);
        
    }
    pulse_pin(sr, 1);

    // direction 1 forward -1 reverse
    uint8_t dir =-1;
    uint8_t brt = 255;
    // uint8_t brt = read_pot(BRT_POT);

    while (!(state_changed(st))) {
        // uint8_t max_brt = read_pot(BRT_POT);
        uint8_t max_brt = 255;

        if (brt > max_brt) {
            brt = max_brt;
            continue;
        }

        // set brightness
        OCR2B = brt;
 
        if (brt > 230) {
            _delay_ms(75);
        } else if (brt > 200) {
            _delay_ms(50);
        } else if (brt > 150) {
            _delay_ms(25);
        } else if (brt < 100) {
            _delay_ms(10);
        } else {
            _delay_ms(10);
        }

        brt += dir;

        if (brt <= 0) {
            brt = 1;
            dir = 1;
        } else if (brt >= max_brt) {
            brt = max_brt;
            dir = -1;
        }
        
        char buf[4];
        itoa(brt, buf, 10);
        lcd_goto_print(0, 9, "   ");
        lcd_goto_print(0, 9, buf);
    }
}