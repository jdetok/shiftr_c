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
        uint8_t max_brt = read_pot(BRT_POT);

        if (brt > max_brt) {
            brt = max_brt;
            continue;
        }
        // set brightness
        OCR2B = brt;

        if (brt > 200) {
            delay_ms_var((read_pot(SPD_POT) * 2)); // delaytime ms
        } else if (brt < 100) {
            delay_ms_var((read_pot(SPD_POT) / 2)); // delaytime ms
        } else {
            delay_ms_var(read_pot(SPD_POT)); // delaytime ms
        }

        // del();
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
    // read_pot(BRT_POT);
   

    // for (uint8_t i = 0; i < off; i++) {
    //     if (state_changed(st)) {return;
    //     // set i to max brightness if it's <
    //     } else if (i < read_pot(BRT_POT)) {
    //         i = read_pot(BRT_POT);
    //         continue;
    //     } else if (i == (off - 1)) { // i set as brightness
    //         for (uint8_t r = i; r > 0; r--) {
    //             if (state_changed(st)) {
    //                 return;
    //             } else if (r > read_pot(BRT_POT)) {
    //                 r = read_pot(BRT_POT);
    //                 continue;
    //             } else {
    //                 OCR2B = r; 
    //                 del();
    //             }
    //         }
    //     } else {
    //         OCR2B = i; 
    //         del();
    //     }   
    // }
// }

