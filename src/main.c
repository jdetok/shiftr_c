#include <avr/io.h> 
#include <util/delay.h>
// #include <stdio.h>
#include "../lib/shift.h"
#include "../lib/ui.h"
// #include "../lib/d13.h"
// #include "../lib/uart.h"
#define NUMREG 4

int main() {
    // digital pins for switches
    switches sw;
    switch_init(&sw);

    // setup digital pins (d4, d6, d7, d8) for shift register
    shiftReg sr;
    shift_init(&sr);

    // setup digital d13 pins - red d9 OCR1A | green d10 OCR1B| blue d11 OCR2A
    // d13LED d13;
    // d13_pwm(&d13);

    // setup analog pins (ADC0 - ADC5) A0-A5 on arduinos
    pot_init();

    // start timing for pwm

    uint32_t now = 0;

    // MAIN LOOP
    while (1) {    
        now++;
        // onoff(&sr, &sw, NUMREG, 0); // all bits off
        
        // onoff(&sr, &sw, NUMREG, 1); // all bits off
        
        // chaser(&sr, &sw, NUMREG, get_state(sw.d9_sw, 'd'));
        // chaser(&sr, &sw, NUMREG, 0);

        if (get_state(sw.d9_sw)) {
            // run chaser if second switch is on, else all leds on
            if (get_state(sw.d10_sw)) { 
                if (get_state(sw.d11_sw)) { // check if intensity switch on
                    // byte_chaser(&sr, &sw, 6);
                chaser(&sr, &sw, NUMREG, get_state(sw.d12_sw));
                } else {
                    chaser(&sr, &sw, read_d11(3), get_state(sw.d12_sw));
                }
            // d10uence off, all lights on
            } else {
                onoff(&sr, &sw, NUMREG, 1); // all bits on
            }
        } else {
            onoff(&sr, &sw, NUMREG, 0); // all bits off
        }
    }
}
