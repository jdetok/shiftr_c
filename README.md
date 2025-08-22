# shift register testing board project in c

## hardware
- the Makefile uploads to an atmega328p chip on an Arduino nano
- leds driven by 74HC595 serial-in-parallel-out 8-bit shift registers
- intend to hookup display for serial comms

## software
- the code is written in plain c without the arduino framework to keep it as lightweight as possible
- avrdude & avr-gcc are required to compile upload to the arduino
