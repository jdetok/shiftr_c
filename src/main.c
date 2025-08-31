#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift/shift.h"
#include "../lib/ui/ui.h"
#include "../lib/lcd/lcd.h"

#define LCD 1 // change to 0 if no lcd screen connected

int main() {
    // digital pins for switches
    switches sw;
    switch_init(&sw);

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

        // if (!get_state(sw.pwr_sw, 'd')) { // pwr_sw off
        if (!switch_state(&sw, PWR_SW)) { // pwr_sw off
            // rgb_off();
            onoff(&sr, &sw, 6, 0, LCD); // all bits off
            continue; // break loop
        }

        // TODO: move chase mode logic to separate func
        // div pot should edit switch between byte chaser and bit chaser/div
        // use existing 2 bit state selector
        if (switch_state(&sw, SEQ_SW)) { // mode switch 
            if (switch_state(&sw, MOD_SW)) { // div switch        
                // byte_chaser(&sr, &sw, NUM_SR, switch_state(&sw, REV_SW));
            //    chaser(&sr, &sw, get_div_pot(sw.state), switch_state(&sw, REV_SW));
            //    ;
                mode_selector(&sr, &sw, get_mod_pot(sw.state), LCD);
            } else {
                chaser(&sr, &sw, NUM_SR, switch_state(&sw, REV_SW), LCD);
            } 
        // sequence off, all lights on
        } else {
            onoff(&sr, &sw, NUM_SR, 1, LCD); // all bits on
        }
    }
}