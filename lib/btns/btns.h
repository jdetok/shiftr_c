// #ifndef BTNS_H
// #define BTNS_H

// #include <stdlib.h>
// #include <util/delay.h>
// #include "../lcd/lcd.h"

// // KEEP D13 FOR POWER SW, BUTTONS D9-D12

// // inputs shift register 74HC165 pins
// #define CE_PIN (1 << PB1) // D9 | chip pin 15 
// #define DATA_PIN (1 << PB2) // D10 | chip pin 7
// #define CLK_PIN (1 << PB3) // D11 | chip pin 2
// #define PL_PIN (1 << PB4) // D12 | chip pin 1 | referred to as load sometimes

// typedef enum {
//     SH0,
//     SH1,
//     SH2,
//     SH3,
//     SH4,
//     SH5,
//     SH6,
//     SH7,
//     BTN_CNT // number of buttons (8)
// } btn_id;

// typedef struct {
//     uint8_t sr_pos;
//     uint8_t state;
//     char *name;
// } button;

// typedef struct {
//     button btn[8];
//     uint8_t state;
// } buttons;

// uint8_t hc165_read(void);
// static void hc165_pulse(volatile uint8_t *port, uint8_t pin);
// void check_btns(buttons *btns);
// void btns_init(buttons *btns);

// #endif