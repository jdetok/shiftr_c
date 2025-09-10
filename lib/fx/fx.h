#ifndef FX_H
#define FX_H

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lcd/lcd.h"
#include "../state/state.h"
#include "../shift/shift.h"

// run appropriate function based on button state
void mode_switcher(state *st, shiftReg *sr, uint8_t lcd_on);

 // all bits on/off
void leds_off(shiftReg *sr, state *st);
void leds_on(shiftReg *sr, state *st);

// chaser effects
void byte_chaser(shiftReg *sr, state *st, int num_sr, uint8_t rev, uint8_t lcd);
void bit_chaser(shiftReg *sr, state *st, uint8_t num_sr, uint8_t rev);

// solid LED pulse
void pulse(shiftReg *sr, state *st);

#endif