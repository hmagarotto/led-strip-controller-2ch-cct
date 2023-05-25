#include <Arduino.h>
#include "Switch.h"

Switch::Switch(uint8_t pin, uint8_t mode, bool pushButton, bool inversePolarity, uint16_t debouncingTimeMs):
    _pin(pin), _mode(mode), _pushButton(pushButton), _inversePolarity(inversePolarity), _debouncingTimeMs(debouncingTimeMs)
{
}

Switch::~Switch()
{
}

void Switch::setup(SwitchStateChange changeCallback) {
    _changeCallback = changeCallback;
    pinMode(_pin, _mode);
    _lastState = digitalRead(_pin);
}

bool Switch::run() {
    const auto actualState = digitalRead(_pin);
    if (_inDebouncing) {
        // debouncing
        if (actualState == _lastState) {
            _inDebouncing = false;
        } else {
            auto diff = millis() - _debouncingStartTime;
            if (diff >= _debouncingTimeMs) {
                _inDebouncing = false;
                auto invokeCallback =  !_pushButton || (actualState != _inversePolarity && _lastState == _inversePolarity);
                _lastState = actualState;
                if (_changeCallback && invokeCallback) {
                    _changeCallback(_lastState);
                }
            }
        }
    } else {
        // wait for change
        if (actualState != _lastState) {
            _inDebouncing = true;
            _debouncingStartTime = millis();
        }
    }
    if (_inDebouncing) {
        Serial.println("Debouncing....");
    }
    return _inDebouncing;
}
