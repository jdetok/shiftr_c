// REPLACE UI_H - ALL CODE FOR SETTING OR GETTING STATE

#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lcd/lcd.h"

#define PWR_SW (1 << PB5) // digital pin 13
#define PWR_ON (PINB & PWR_SW) // check d13 state

// ADC channels correlating with analog pins
#define BRT_POT 0
#define SPD_POT 1
#define MOD_POT 2
#define RGB_POT 3

// BUTTONS inputs Bift register 74HC165 pins 
#define CE_PIN (1 << PB1) // D9 | chip pin 15 
#define DATA_PIN (1 << PB2) // D10 | chip pin 7
#define CLK_PIN (1 << PB3) // D11 | chip pin 2
#define PL_PIN (1 << PB4) // D12 | chip pin 1 | referred to as load sometimes

// buttons
typedef enum {
    B0, B1, B2, B3, B4, B5, B6, B7,
    BTN_CNT // number of buttons (8)
} btn_id;

// button bit positions
#define BTN1 (1 << B0)
#define BTN2 (1 << B1)
#define BTN3 (1 << B2)
#define BTN4 (1 << B3)
#define BTN5 (1 << B4)
#define BTN6 (1 << B5)
#define BTN7 (1 << B6)
#define BTN8 (1 << B7)

typedef struct {
    uint8_t btn_now; // current buttons reading
    uint8_t btn_state; // persistant button state
} state;

void btns_init(state *st);
void print_state(state *st);
uint8_t read_btns();
uint8_t state_changed(state *st);
uint8_t ison(state *st, uint8_t btn);

// pots
void pot_init(); // setup potentiometers to utilize PWM
void oe_pwm(); // setup PWM channels
uint8_t read_pot(uint8_t channel); // returns 0 - 244

#endif