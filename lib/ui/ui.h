#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h> 
#include <util/delay.h>

#include "../lcd/lcd.h"

// digital pins for switches
// defined with PCB layout - may be different using proto board
#define PWR_PIN (1 << PB5) // digital pin 13
// #define SEQ_PIN (1 << PB4) // digital pin 12
// #define REV_PIN (1 << PB3) // digital pin 11
// #define MOD_PIN (1 << PB2) // digital pin 10
// #define RGB_PIN (1 << PB1) //  digital pin 9

// ADC channels correlating with analog pins
#define BRT_POT 0
#define SPD_POT 3
#define MOD_POT 1
#define RGB_POT 2

// return digital pin states for D and C reg
#define DPIN_STATE(P) ((PIND & (P)) ? 1 : 0)
#define CPIN_STATE(P) ((PINC & (P)) ? 1 : 0)
#define PIN_STATE(REG, PIN) (((REG) & (PIN)) ? 1 : 0)

// bit fields for switch states
#define PWR_BIT 0b1
#define SEQ_BIT 0b10
#define REV_BIT 0b100
#define MOD_BIT 0b1000
#define RGB_BIT 0b10000

// store div pot state in bits 5 & 6 (binary 3 left shifited 5 bits)
#define MOD_POT_SHIFT 5 
#define MOD_POT_MASK (0b11 << MOD_POT_SHIFT)

// BUTTONS inputs shift register 74HC165 pins 
#define CE_PIN (1 << PB1) // D9 | chip pin 15 
#define DATA_PIN (1 << PB2) // D10 | chip pin 7
#define CLK_PIN (1 << PB3) // D11 | chip pin 2
#define PL_PIN (1 << PB4) // D12 | chip pin 1 | referred to as load sometimes

// buttons
typedef enum {
    SH0,
    SH1,
    SH2,
    SH3,
    SH4,
    SH5,
    SH6,
    SH7,
    BTN_CNT // number of buttons (8)
} btn_id;

// enum for switches 
typedef enum {
    PWR_SW,
    SW_COUNT
} switch_id;

// struct holds a PIN, BIT, and char either 'c' or 'd' to denote register
typedef struct {
    uint8_t pin;
    uint8_t bit;
    char reg;
} switch_two_pos;

// struct to hold swithes - holds array of switches & state byte
typedef struct {
    switch_two_pos switches[1];
    uint8_t state; // use bitfields for state
} switches;
// KEEP D13 FOR POWER SW, BUTTONS D9-D12

typedef struct {
    uint8_t sr_pos;
    uint8_t state;
    char *name;
} button;

typedef struct {
    button btn[8];
    uint8_t state;
    uint8_t prev_state;
} buttons;

typedef struct {
    buttons btns;
    switches sw;
} inputs;

inputs ui_init();
uint8_t btns_state(buttons *btns);
uint8_t btn_state(buttons *btns, uint8_t btn);
void pot_init(); // setup potentiometers to utilize PWM
void oe_pwm(); // setup PWM channels
void switch_init(switches *sw); // setup switches
void set_state(uint8_t *state, uint8_t pin, char reg, uint8_t bit); // set bit
uint8_t check_state(switches *sw, uint8_t lcd);
uint8_t switch_state(switches *sw, switch_id id);
uint8_t read_pot(uint8_t channel); // returns 0 - 244
uint8_t read_mod_pot(); // writes 1-3 to bits 5-6 in state byte
uint8_t get_mod_pot(uint8_t state); // gets bits 5-6 for mode_selector (shift.h)
uint8_t read_rgb_brt(); // returns brightness divider for rgb pulse func

uint8_t hc165_read(void);
static void hc165_pulse(volatile uint8_t *port, uint8_t pin);
void check_btns(buttons *btns);
void btns_init(buttons *btns);

#endif