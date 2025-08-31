# arduino pinout
## THIS WILL CHANGE - WILL EDIT PINOUT & ADD SCREEN

# ui pins
## switches
- d9
    - alt switch no purpose now
        - rgb switch on bronto
    - PB1
- d10 
    - SEQUENCE MODE SWITCH
    - when on, reads sequence mode pot
    - PB2
- d11
    - REVERSE SEQUENCE SWITCH
    - PB3
- d12
    - SEQUENCE SWITCH ON/SEQUENCE
    - PB4
- d13
    - POWER SWITCH
    - PB5
## potentiometers
- shift register LED brightness pot
    - a0 | PC0 | adc chan. 0
- seq mode pot
    - chaser mode when seq mode switch is on
    - a1 | PC1 | adc chan. 1
- alt pot
    - a2 | PC2 | adc chan. 2
- speed pot | speed for chaser effect 
    - a3 | PC3 | adc chan. 3
# 74HC595 shift register pins
- serial pin | set a bit in the register
    - d2 | PD2
- output enable (oe) pin
    - d3 | PD3 | OCR2B = x to set shift register LED brightness
    * pwm, controls led brightness
- clock pin 
    - d4 | PD4 | set current bits to register
- latch pin
    - d5 | PD5 | output bits in shift register to memory register