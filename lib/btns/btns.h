#ifndef BTNS_H
#define BTNS_H

#include "../lcd/lcd.h"

// KEEP D13 FOR POWER SW, BUTTONS D9-D12

// inputs shift register 74HC165 pins
#define CE_PIN (1 << PB1) // D9 | chip pin 15 
#define DATA_PIN (1 << PB2) // D10 | chip pin 7
#define CLK_PIN (1 << PB3) // D11 | chip pin 2
#define PL_PIN (1 << PB4) // D12 | chip pin 1 | referred to as load sometimes

#endif