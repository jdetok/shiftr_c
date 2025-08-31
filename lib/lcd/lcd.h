// lcd.h
#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

// SER (data) = D6 = PD6
#define SR_SER_PORT   PORTD
#define SR_SER_DDR    DDRD
#define SR_SER_PIN    PD6

// CLK = D8 = PB0
#define SR_CLK_PORT  PORTD
#define SR_CLK_DDR    DDRD
#define SR_CLK_PIN    PD7

// LATCH = D7 = PD7
#define SR_LATCH_PORT PORTB
#define SR_LATCH_DDR  DDRB
#define SR_LATCH_PIN  PB0

// LCD API
void lcd_init(void);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_print(const char *str);
void lcd_goto(uint8_t row, uint8_t col);
void lcd_clear(void);

#endif
