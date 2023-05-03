#include <cmath>
#include "Arduino.h"
#include <gamma_correction.h>
#include "DimmableLight.h"

DimmableLight::DimmableLight(uint8_t pin, bool inversePolarity) :
    _pin(pin), _inversePolarity(inversePolarity)
{
}

DimmableLight::~DimmableLight()
{
}

void DimmableLight::setup() {
    pinMode(_pin, OUTPUT);
    analogWriteResolution(10);
    setHardware();
}

void DimmableLight::setState(const LightState& state) {
    _desired.on = state.on;
    _desired.bri = map(state.bri, 0, 255, 0, 1024);
    uint16_t totalDiff = 0;
    if (_desired.on) {
        totalDiff =  abs(_desired.bri - _current.bri);
    } else {
        totalDiff = _current.bri;
    }
    _transitionSteps = 0;
    do {
        _transitionSteps++;
        _transitionDelayMs = _transitionSteps * _transitionTimeMs / totalDiff;
    } while (_transitionDelayMs<10);
    Serial.printf("totalDiff:[%hu] transitionSteps:[%hhu] transitionDelayMs:[%hu]\n", totalDiff, _transitionSteps, _transitionDelayMs);
    _nextChangeAtMs = millis() + _transitionDelayMs;
    _running = true;
}

bool DimmableLight::run() {
    if (!_running) {
        return false;
    }
    if (millis() > _nextChangeAtMs) {
        return false;
    }
    if (_desired.on) {
        if (_current.bri < _desired.bri) {
            _current.bri+=_transitionSteps;
            if (_current.bri > _desired.bri) _current.bri = _desired.bri;
        } else if (_current.bri > _desired.bri) {
            _current.bri-=_transitionSteps;
            if (_current.bri < _desired.bri) _current.bri = _desired.bri;
        }
        if (_current.bri == _desired.bri) {
            _current.on = _desired.on;
            _running = false;
        }
    } else {
        if (_current.bri > 0) {
            _current.bri-=_transitionSteps;
            if (_current.bri < 0) _current.bri = 0;
        }
        if (_current.bri == 0) {
            _current.on = _desired.on;
            _running = false;
        }
    }
    setHardware();
    _nextChangeAtMs = millis() + _transitionDelayMs;
    return true;
}

void DimmableLight::setHardware() {
    Serial.println(_current.bri);
    int bri = pgm_read_word(&gamma_correction::lut[_current.bri]);
    int pwmValue = _inversePolarity ? gamma_correction::max - bri : bri;
    analogWriteMode(_pin, pwmValue, _inversePolarity);
}
