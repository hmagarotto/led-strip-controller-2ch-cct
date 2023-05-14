#include <interrupts.h>
#include "HueDevices.h"

void HueDevices::setup(const Config::DTO::Config& config) {
    lightList.clear();
    lightList.reserve(config.hardware.lights.size());
    for (auto light : config.hardware.lights) {
        lightList.push_back({
            type: light.type,
            device: createLightDevice(light),
            state: createLightState(light),
        });
    }
}

LightDevice HueDevices::createLightDevice(const Config::DTO::Light& lightConfig) {
    LightDevice device;
    device.name = (char*)malloc(lightConfig.friendlyName.length() + 1);
    memcpy((void*)device.name, lightConfig.friendlyName.c_str(), lightConfig.friendlyName.length() + 1);
    device.uniqueid = (char*)malloc(lightConfig.uniqueId.length() + 1);
    memcpy((void*)device.uniqueid, lightConfig.uniqueId.c_str(), lightConfig.uniqueId.length() + 1);
    return device;
}

LightState HueDevices::createLightState(const Config::DTO::Light& lightConfig) {
    LightState state;
    switch (lightConfig.type) {
        case Config::DTO::LightType::SIMPLE:
        case Config::DTO::LightType::DIMMABLE:
            state.colormode[0] = '\0';
            break;
        case Config::DTO::LightType::CCT:
            memcpy(state.colormode, "ct", 3);
            break;
    }
    return state;
}

uint8_t HueDevices::getLightNum() {
    return this->lightList.size();
}

LightState HueDevices::getLightState(uint8_t index) {
    assert(index < getLightNum());
    esp8266::InterruptLock lock;
    return lightList[index].state;
}

LightState HueDevices::updateLightState(uint8_t index, LightState& update, uint8_t fields) {
    assert(index < getLightNum());
    esp8266::InterruptLock lock;
    LightState* state = &lightList[index].state;
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

const LightDevice& HueDevices::getLightDevice(uint8_t index) {
    assert(index < getLightNum());
    return lightList[index].device;
}
