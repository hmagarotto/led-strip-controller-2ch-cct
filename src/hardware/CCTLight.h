#ifndef CCTLIGHT_H
#define CCTLIGHT_H

#pragma once

#include <cstddef>
#include "LightBase.h"

class CCTLight : public LightBase
{
public:
    static constexpr size_t channels = 2;

    CCTLight(const uint8_t pin[channels], const bool inversePolarity[channels], uint8_t ctBlend = 50, bool gammaCorrection = true, uint16_t transitionTimeMs = 2000);
    virtual ~CCTLight();

    virtual void setup();
    virtual void setState(const LightState& state);
    virtual bool run();

protected:
    bool run(size_t ch);
    void setHardware(size_t ch);

    static constexpr size_t channel_cw = 0;
    static constexpr size_t channel_ww = 1;

    // config
    uint8_t _pin[channels];
    bool _inversePolarity[channels];
    uint8_t _ctBlend; // 0 - linear (CCT 127 = 50% warm, 50% cold), 127 - additive CCT blending (CCT 127 = 100% warm, 100% cold)
    bool _gammaCorrection;
    uint16_t _transitionTimeMs;

    // control
    struct Control {
        bool running = false;
        uint8_t transitionInc;
        uint16_t transitionDelayMs;
        unsigned long nextChangeAtMs;
        unsigned long start;
    } _control[channels];

    // state
    bool _on = false;
    struct State {
        int16_t currentBri = 0;
        int16_t desiredBri = 0;
    } _state[channels];
};

#endif
