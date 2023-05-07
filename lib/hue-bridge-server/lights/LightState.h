#ifndef LIGHTSTATE_H
#define LIGHTSTATE_H

#pragma once

#include <cstdint>

struct LightState
{
    bool on = false;
    uint8_t bri = 52;
    uint16_t hue = 0;
    uint8_t sat = 0;
    float xy[2] = {0, 0};
    uint16_t ct = 284;
    char colormode[3] = "ct";
    char alert[16] = "none";
    char mode[16] = "homeautomation";
    bool reachable = true;
};

enum LightStateUpdateField {
  on  = 0x01,
  bri = 0x02,
  hue = 0x04,
  sat = 0x08,
  xy  = 0x10,
  ct  = 0x20
};

#endif