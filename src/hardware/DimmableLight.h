#ifndef DIMMABLELIGHT_H
#define DIMMABLELIGHT_H

#pragma once

#include "LightBase.h"

class DimmableLight : public LightBase
{
public:
    DimmableLight(uint8_t pin, bool inversePolarity, uint16_t transitionTimeMs = 2000);
    virtual ~DimmableLight();

    virtual void setup();
    virtual void setState(const LightState& state);
    virtual bool run();

protected:
    void setHardware();

    // config
    uint8_t _pin;
    bool _inversePolarity;
    uint16_t _transitionTimeMs;

    // control
    bool _running = false;
    uint8_t _transitionInc;
    uint16_t _transitionDelayMs;
    unsigned long _nextChangeAtMs;

    // state
    bool _on = false;
    int16_t _currentBri = 0;
    int16_t _desiredBri = 0;
};

#endif