#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <Arduino.h>
#include <vector>

namespace Config::DTO {

enum LightType {
    SIMPLE,
    DIMMABLE,
    CCT,
};

enum LightPowerOnState {
    ON,
    OFF,
    LAST_STATE,
};

enum SwitchAction {
    TOGGLE_ALL_LIGHTS,
};

struct Light {
    LightType type;
    String friendlyName;
    String uniqueId;
    std::vector<uint8_t> pins;
    std::vector<bool> inversePolarity;
    LightPowerOnState powerOnState;
};

struct Switch {
    uint8_t pin;
    bool pushButton;
    bool inversePolarity;
    SwitchAction action;
};

struct Hardware
{
    std::vector<Light> lights;
    std::vector<Switch> switches;
};

struct LightSync {
    uint8_t lightIndex;
    uint8_t remoteLightIndex;
    uint8_t remoteMAC[6];
};

struct Sync {
    std::vector<LightSync> lights;
};

struct Config
{
    Hardware hardware;
    Sync sync;
};

}
#endif