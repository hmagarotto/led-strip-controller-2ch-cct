#ifndef DIMMABLELIGHT_H
#define DIMMABLELIGHT_H

#pragma once

#include "LightBase.h"

class DimmableLight : public LightBase
{
public:
    DimmableLight(uint8_t pin, bool inversePolarity);
    virtual ~DimmableLight();

    virtual void setup();
    virtual void setState(const LightState& state);
    virtual bool run();

protected:
    void setHardware();
    uint8_t _pin;
    bool _inversePolarity = false;
    bool _running = false;
    uint16_t _transitionTimeMs = 4000;
    uint8_t _transitionSteps;
    uint16_t _transitionDelayMs;
    unsigned long _nextChangeAtMs;
    struct State {
        bool on;
        int16_t bri;
    } _current = {false, 0}, _desired = {false, 0};
};

#endif