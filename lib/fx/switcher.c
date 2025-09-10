#include "fx.h"

// turn off all lights // lcd 0 or 1 to enable prints
void onoff(shiftReg *sr, state *st, int num_sr, int on, uint8_t lcd) {
    int bits = num_sr * 8;
    for (int i = 0; i < bits; i++) {
        // uint8_t interrupt = update_states(sw);
        if (state_changed(st)) {
            return;
        } else {
            set_brt();
            if (on) {
                PORTD |= sr->ser;
            } else {
                PORTD &= ~sr->ser;
            }
            pulse_pin(sr, 0); // pulse clock
        }
    }
    pulse_pin(sr, 1); // pulse latch
}

void mode_switcher(state *st, shiftReg *sr, uint8_t lcd_on) {
    /* this function serves as the main mode switcher using button state
    this should be called directly after checking the power switch (PWR_ON macro)
    if BTN1 is not pressed, all lEDs willrm  turn on & the function will return
    if BTN1 is pressed, BTN2 will determine whether a sequence or pulse function
    runs. as of the writing of this (morning 9/10/25) no pulsing functions have
    been written
    */
   if (!(ison(st, BTN1))) {
            onoff(sr, st, NUM_SR, 1, lcd_on);
            return;
        } else {
            // this is where mode_switcher will be called
            if (!(ison(st, BTN3))) {
                bit_chaser(sr, st, NUM_SR, ison(st, BTN8));
            } else {
                byte_chaser(sr, st, NUM_SR, ison(st, BTN8), 0);
            }
        }

}