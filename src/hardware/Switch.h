#ifndef SWITCH_H
#define SWITCH_H

#pragma once

#include <cstdint>
#include <functional>

class Switch
{
public:
    typedef std::function<void(int state)> SwitchStateChange;

    Switch(uint8_t pin, uint8_t mode, bool inversePolarity = false, uint16_t debouncingTimeMs = 30);
    virtual ~Switch();

    virtual void setup(SwitchStateChange changeCallback);
    virtual bool run();

protected:
    // config
    uint8_t _pin;
    uint8_t _mode;
    bool _inversePolarity;
    uint16_t _debouncingTimeMs;

    // control
    SwitchStateChange _changeCallback;
    int _lastState;
    bool _inDebouncing = false;
    unsigned long _debouncingStartTime;
};

#endif
