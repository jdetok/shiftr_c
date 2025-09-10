#include "fx.h"

void pulse() {
    OCR2B = read_pot(BRT_POT); // set brightness
}

