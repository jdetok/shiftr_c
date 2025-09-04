#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift/shift.h"
#include "../lib/ui/ui.h"
#include "../lib/lcd/lcd.h"
// #include "../lib/btns/btns.h"

#define LCD 1 // change to 0 if no lcd screen connected

int main() {
    // digital pins for switches
    switches sw;
    switch_init(&sw);

    buttons btns;
    btns_init(&btns);

    inputs ui = ui_init();

    // setup digital pins (d4, d6, d7, d8) for shift register
    shiftReg sr;
    shift_init(&sr);

    // setup analog pins (ADC0 - ADC5) A0-A5 on arduinos
    pot_init();

    // start timing for pwm
    uint32_t now = 0;
    if (LCD == 1) {
        lcd_init();
        lcd_clear();
        // lcd_print("hello");
    }

    // MAIN LOOP
    while (1) {
        now++; // increment timing
        // set_brt();
        // check_btns(&btns);
        btns_state(&ui.btns);
        char binval[9];
        for (int b = 7; b >= 0; b--) {
            binval[7 - b] = (ui.btns.state & (1 << b)) ? '1' : '0';
        }
        binval[8] = '\0';

        lcd_goto_print(0, 0, binval);
        // if (!get_state(sw.pwr_sw, 'd')) { // pwr_sw off
        if (!switch_state(&ui.sw, PWR_SW)) { // pwr_sw off
            onoff(&sr, &ui, 6, 0, LCD); // all bits off
            continue; // break loop
        } else {
            // if (btns.state & btns.btn[0].sr_pos) {
            if (ui.btns.state & ui.btns.btn[0].sr_pos) {
            // if (btn_state(&btns, SH0)) {
                byte_chaser(&sr, &ui, NUM_SR, (ui.btns.state & ui.btns.btn[6].sr_pos), 0);
            } else if (ui.btns.state & ui.btns.btn[7].sr_pos) {
                chaser(&sr, &ui, NUM_SR, (ui.btns.state & ui.btns.btn[6].sr_pos), 0);
            } else {
                onoff(&sr, &ui, NUM_SR, 1, LCD); // all bits on
            }
        }

        // TODO: move chase mode logic to separate func
        // div pot should edit switch between byte chaser and bit chaser/div
        // use existing 2 bit state selector
        // if (switch_state(&sw, SEQ_SW)) { // mode switch 
        //     if (switch_state(&sw, MOD_SW)) { // div switch        
        //         mode_selector(&sr, &sw, get_mod_pot(sw.state), LCD);
        //     } else {
        //         chaser(&sr, &sw, NUM_SR, switch_state(&sw, REV_SW), LCD);
        //     } 
        // // sequence off, all lights on
        // } else {
        //     onoff(&sr, &sw, NUM_SR, 1, LCD); // all bits on
        }
    }
// }