#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include "LightBase.h"

class Light : public LightBase
{
public:
    Light();
    virtual ~Light();

    virtual void setup();
    virtual void setState(const LightState& state);
    virtual bool run();

private:
    uint8_t _pin;
    bool _on = false;
    bool _inversePolarity = false;

    bool physicalState(bool logicalState) {
        return _inversePolarity != logicalState;
    }
};

#endif