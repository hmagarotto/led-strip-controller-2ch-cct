#include <cmath>
#include "Arduino.h"
#include <gamma_correction.h>
#include "CCTLight.h"

CCTLight::CCTLight(const uint8_t pin[channels], const bool inversePolarity[channels], uint8_t ctBlend, bool gammaCorrection, uint16_t transitionTimeMs) :
    _ctBlend(ctBlend),
    _gammaCorrection(gammaCorrection),
    _transitionTimeMs(transitionTimeMs)
{
    for (size_t ch=0; ch<channels; ch++) {
        _pin[ch] = pin[ch];
        _inversePolarity[ch] = inversePolarity[ch];
    }
}

CCTLight::~CCTLight()
{
}

void CCTLight::setup() {
    analogWriteResolution(10);
    for (size_t ch=0; ch<channels; ch++) {
        pinMode(_pin[ch], OUTPUT);
        setHardware(ch);
    }
}

void CCTLight::setState(const LightState& state) {
    _on = state.on;

    // state.ct: 153 (max cold white) and 500 (max warm white)
    // cct: 0 - full warm white, 255 - full cold white
    uint8_t cct = 255 - ((state.ct-153) * 255 / 347);
    uint8_t ww, cw;
    // ww
    if (cct < _ctBlend) ww = 255;
    else ww = ((255-cct) * 255) / (255 - _ctBlend);
    // cw
    if ((255-cct) < _ctBlend) cw = 255;
    else cw = (cct * 255) / (255 - _ctBlend);
    // brightness scaling
    ww = (state.bri * ww) / 255;
    cw = (state.bri * cw) / 255;

    // desired
    _state[channel_ww].desiredBri = map(ww, 0, 255, 0, 1024);
    _state[channel_cw].desiredBri = map(cw, 0, 255, 0, 1024);

    // transition control
    for (size_t ch=0; ch<channels; ch++) {
        uint16_t totalDiff = 0;
        auto& desiredBri = _state[ch].desiredBri;
        auto& currentBri = _state[ch].currentBri;
        auto& running = _control[ch].running;
        auto& transitionInc = _control[ch].transitionInc;
        auto& transitionDelayMs = _control[ch].transitionDelayMs;
        auto& nextChangeAtMs = _control[ch].nextChangeAtMs;
        auto& start = _control[ch].start;
        if (_on) {
            totalDiff =  abs(desiredBri - currentBri);
        } else {
            totalDiff = currentBri;
        }
        if (totalDiff == 0) {
            running = false;
            continue;
        }
        transitionInc = 0;
        transitionDelayMs = 0;
        if (totalDiff-1 > 0) {
            do {
                transitionInc++;
                transitionDelayMs = transitionInc * _transitionTimeMs / (totalDiff-1);
            } while (transitionDelayMs<10);
            if (transitionDelayMs > _maxTransitionDelayMs) {
                transitionDelayMs = _maxTransitionDelayMs;
            }
        }
        Serial.printf("ch[%u] current:[%u] desired:[%u]\n", ch, currentBri, desiredBri);
        Serial.printf("ch[%u] totalDiff:[%hu] transitionSteps:[%hhu] transitionDelayMs:[%hu]\n", ch, totalDiff, transitionInc, transitionDelayMs);
        start = nextChangeAtMs = millis();
        running = true;
    }
}

bool CCTLight::run() {
    bool result = false;
    for (size_t ch=0; ch<channels; ch++) {
        result = run(ch) || result;
    }
    if (result) {
        Serial.printf("COLD=>%hu WARM=>%hu\r\n", _state[0].currentBri, _state[1].currentBri);
    }
    return result;
}

bool CCTLight::run(size_t ch) {
    auto& running = _control[ch].running;
    if (!running) {
        return false;
    }

    auto& nextChangeAtMs = _control[ch].nextChangeAtMs;
    if (millis() < nextChangeAtMs) {
        return false;
    }

    auto& desiredBri = _state[ch].desiredBri;
    auto& currentBri = _state[ch].currentBri;
    auto& transitionInc = _control[ch].transitionInc;
    auto& transitionDelayMs = _control[ch].transitionDelayMs;

    if (_on) {
        if (currentBri < desiredBri) {
            currentBri+=transitionInc;
            if (currentBri > desiredBri) currentBri = desiredBri;
        } else if (currentBri > desiredBri) {
            currentBri-=transitionInc;
            if (currentBri < desiredBri) currentBri = desiredBri;
        }
        if (currentBri == desiredBri) {
            running = false;
        }
    } else {
        if (currentBri > 0) {
            currentBri-=transitionInc;
            if (currentBri < 0) currentBri = 0;
        }
        if (currentBri == 0) {
            running = false;
        }
    }
    setHardware(ch);
    nextChangeAtMs = millis() + transitionDelayMs;
    if (!running) {
        auto diff = millis() - _control[ch].start;
        Serial.printf("Runtime[%u]: %lums\n", ch, diff);
    }
    return true;
}

void CCTLight::setHardware(size_t ch) {
    int bri = _gammaCorrection ?
        pgm_read_word(&gamma_correction::lut[_state[ch].currentBri]) :
        _state[ch].currentBri;
    int pwmValue = _inversePolarity[ch] ? 1023 - bri : bri;
    analogWriteMode(_pin[ch], pwmValue, _inversePolarity[ch]);
}
