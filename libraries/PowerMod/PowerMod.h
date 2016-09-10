#ifndef PowerMod_h
#define PowerMod_h

#include <Arduino.h>

uint32_t mul_mod(uint32_t a, uint32_t b, uint32_t m);

uint32_t pow_mod(uint32_t b, uint32_t e, uint32_t m);

#endif