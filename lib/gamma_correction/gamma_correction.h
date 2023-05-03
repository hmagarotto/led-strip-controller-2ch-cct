#ifndef gamma_lut_h
#define gamma_lut_h

#pragma once

#include <cstdint>
#include <pgmspace.h>

namespace gamma_correction {

// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 2.00 steps = 1024 range = 0-1023

extern const float value;
extern const uint16_t steps;
extern const uint16_t min;
extern const uint16_t max;
extern const uint16_t lut[] PROGMEM;

}

#endif
