#ifndef LIGHTBASE_H
#define LIGHTBASE_H

#pragma once

#include <lights/LightState.h>

class LightBase
{
public:
    LightBase() {}
    virtual ~LightBase() {}

    virtual void setup() = 0;
    virtual void setState(const LightState& state) = 0;
    virtual bool run() = 0;
};

#endif