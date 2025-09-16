#include <avr/io.h> 
#include <util/delay.h>
#include "../lib/shift/shift.h"
#include "../lib/state/state.h"
#include "../lib/lcd/lcd.h"
#include "../lib/fx/fx.h"

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
            leds_off(&sr, &st);
            continue;
        } else {
            mode_switcher(&st, &sr, LCD);
            // bit_chaser(&sr, &st, NUM_SR, ison(&st, BTN8));
        }
    }
}