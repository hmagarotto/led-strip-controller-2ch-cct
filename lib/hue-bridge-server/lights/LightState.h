#ifndef LIGHTSTATE_H
#define LIGHTSTATE_H

#pragma once

#include <cstdint>

struct LightState
{
    bool on;
    uint8_t bri;
    uint16_t hue;
    uint8_t sat;
    float xy[2];
    uint16_t ct;
    char colormode[3];
    char alert[16];
    char mode[16];
    bool reachable;
};

#endif