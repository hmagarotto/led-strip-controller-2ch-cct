#include <cmath>
#include "Arduino.h"
#include <gamma_correction.h>
#include "DimmableLight.h"


DimmableLight::DimmableLight(uint8_t pin, bool inversePolarity, uint16_t transitionTimeMs) :
    _pin(pin), _inversePolarity(inversePolarity), _transitionTimeMs(transitionTimeMs)
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
    _on = state.on;
    _desiredBri = map(state.bri, 0, 255, 0, 1024);
    uint16_t totalDiff = 0;
    if (_on) {
        totalDiff =  abs(_desiredBri - _currentBri);
    } else {
        totalDiff = _currentBri;
    }
    if (totalDiff == 0) {
        _running = false;
        return;
    }
    _transitionInc = 0;
    do {
        _transitionInc++;
        _transitionDelayMs = _transitionInc * _transitionTimeMs / totalDiff;
    } while (_transitionDelayMs<10);
    Serial.printf("totalDiff:[%hu] transitionSteps:[%hhu] transitionDelayMs:[%hu]\n", totalDiff, _transitionInc, _transitionDelayMs);
    _nextChangeAtMs = millis();
    _running = true;
}

bool DimmableLight::run() {
    if (!_running) {
        return false;
    }
    if (millis() < _nextChangeAtMs) {
        return false;
    }
    if (_on) {
        if (_currentBri < _desiredBri) {
            _currentBri+=_transitionInc;
            if (_currentBri > _desiredBri) _currentBri = _desiredBri;
        } else if (_currentBri > _desiredBri) {
            _currentBri-=_transitionInc;
            if (_currentBri < _desiredBri) _currentBri = _desiredBri;
        }
        if (_currentBri == _desiredBri) {
            _running = false;
        }
    } else {
        if (_currentBri > 0) {
            _currentBri-=_transitionInc;
            if (_currentBri < 0) _currentBri = 0;
        }
        if (_currentBri == 0) {
            _running = false;
        }
    }
    setHardware();
    _nextChangeAtMs = millis() + _transitionDelayMs;
    return true;
}

void DimmableLight::setHardware() {
    Serial.println(_currentBri);
    int bri = pgm_read_word(&gamma_correction::lut[_currentBri]);
    int pwmValue = _inversePolarity ? gamma_correction::max - bri : bri;
    analogWriteMode(_pin, pwmValue, _inversePolarity);
}
