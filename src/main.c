#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift/shift.h"
#include "../lib/state/state.h"
#include "../lib/lcd/lcd.h"
// #include "../lib/btns/btns.h"

#define LCD 1 // change to 0 if no lcd screen connected

int main() {
    // digital pins for switches
    // switches sw;
    // switch_init(&sw);

    // buttons btns;
    // btns_init(&btns);

    // inputs ui = ui_init();

    // setup digital pins (d4, d6, d7, d8) for shift register
    shiftReg sr;
    shift_init(&sr);

    state st;
    state_init(&st);
    btns_init();
    // setup analog pins (ADC0 - ADC5) A0-A5 on arduinos
    pot_init();

    // start timing for pwm
    uint32_t now = 0;
    if (LCD == 1) {
        lcd_init();
        lcd_clear();
    }

    // MAIN LOOP
    while (1) {
        now++; // increment timing

        char binval[9];
        for (int b = 7; b >= 0; b--) {
            binval[7 - b] = (st.btn_state & (1 << b)) ? '1' : '0';
        }
        binval[8] = '\0';

        lcd_goto_print(0, 0, binval);
        // if (!get_state(sw.pwr_sw, 'd')) { // pwr_sw off
        if (!(PWR_ON)) { // pwr_sw off
            onoff(&sr, &st, 6, 0, LCD); // all bits off
            continue; // break loop
        }
            // if (btns.state & btns.btn[0].sr_pos) {
        if (st.btn_state & (1 << B0)) {
        // if (btn_state(&btns, SH0)) {
            byte_chaser(&sr, &st, NUM_SR, (st.btn_state & (1 << B6)), 0);
        } else if (st.btn_state & (1 << B7)) {
            bit_chaser(&sr, &st, NUM_SR);
            // chaser(&sr, &st, NUM_SR, (st.btn_state & (1 << B6)), 0);
        } else {
            onoff(&sr, &st, NUM_SR, 1, LCD); // all bits on
        }
    }
}