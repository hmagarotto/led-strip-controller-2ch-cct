#include <interrupts.h>
#include "DevicesConfig.h"

const LightDevice DevicesConfig::lightDeviceList[DevicesConfig::lightNum] = {
    { name: "kitchen bench light",  uniqueid: "00:17:88:f1:00:11:ed:b3-0b" }, 
    { name: "kitchen stove light",  uniqueid: "00:17:88:a1:00:c0:6d:03-0b" }
};

LightState DevicesConfig::lightStateList[DevicesConfig::lightNum] = {};

LightState DevicesConfig::getLightState(uint8_t index) {
    assert(index < lightNum);
    esp8266::InterruptLock lock;
    return lightStateList[index];
}

LightState DevicesConfig::updateLightState(uint8_t index, LightState& update, uint8_t fields) {
    assert(index < lightNum);
    esp8266::InterruptLock lock;
    LightState* state = &lightStateList[index];
    if (fields & LightStateUpdateField::on)     state->on = update.on;
    if (fields & LightStateUpdateField::bri)    state->bri = update.bri;
    if (fields & LightStateUpdateField::hue)    state->hue = update.hue;
    if (fields & LightStateUpdateField::sat)    state->hue = update.sat;
    if (fields & LightStateUpdateField::ct)     state->ct = update.ct;
    if (fields & LightStateUpdateField::xy) {
        state->xy[0] = update.xy[0];
        state->xy[1] = update.xy[1];
    }
    return *state;
}
