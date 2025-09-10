#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift/shift.h"
#include "../lib/state/state.h"
#include "../lib/lcd/lcd.h"
// #include "../lib/btns/btns.h"

#define LCD 1 // change to 0 if no lcd screen connected

int main() {
    // init shift reg struct, sets its pins as outputs
    shiftReg sr;
    shift_init(&sr);

    // setup state struct for buttons
    state st;
    btns_init(&st);
    
    // setup pins A0-A3 for analog readings
    pot_init();
    
    // setup LCD if 1
    if (LCD) {
        lcd_init();
        lcd_clear();
    }

    // start timing for pwm
    uint32_t now = 0;

    // MAIN LOOP
    while (1) {
        now++; // increment timing

        // print button state
        print_state(&st);
        
        // all LEDs off if power switch is off
        if (!(PWR_ON)) {
            onoff(&sr, &st, NUM_SR, 0, LCD);
            continue;
        }

        // read buttons for sequencing
        if (st.btn_state & (1 << B0)) {
        // if (btn_state(&btns, SH0)) {
            byte_chaser(&sr, &st, NUM_SR, (st.btn_state & (1 << B6)), 0);
        } else if (st.btn_state & (1 << B7)) {
            bit_chaser(&sr, &st, NUM_SR, (st.btn_state & (1 << B6)));
            // chaser(&sr, &st, NUM_SR, (st.btn_state & (1 << B6)), 0);
        } else {
            onoff(&sr, &st, NUM_SR, 1, LCD); // all bits on
        }
    }
}