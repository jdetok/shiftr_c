#include "fx.h"

void leds_off(shiftReg *sr, state *st) {
    lcd_goto_print(0, 0, "LEDs off\0");
    for (uint8_t i = 0; i < BITS; i++) {
        if (state_changed(st)) {
            return;
        } else {
            PORTD &= ~(sr->ser);
            pulse_pin(sr, 0);
        }
    }
    pulse_pin(sr, 1);
}

void leds_on(shiftReg *sr, state *st) {
    lcd_goto_print(0, 0, "LEDs on!\0");
    for (uint8_t i = 0; i < BITS; i++) {
        if (state_changed(st)) {
            return;
        } else {
            set_brt();
            PORTD |= sr->ser;
            pulse_pin(sr, 0);
        }
    }
    pulse_pin(sr, 1);
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
        leds_on(sr, st);
        return;
   } else {
        if (!(ison(st, BTN3))) {
            bit_chaser(sr, st, NUM_SR, ison(st, BTN8));
        } else {
            byte_chaser(sr, st, NUM_SR, ison(st, BTN8), 0);
        }
    }

}