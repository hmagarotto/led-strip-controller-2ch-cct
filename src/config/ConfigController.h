#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

#pragma once

#include <ESPAsyncWebServer.h>
#include "config/dto/Config.h"

class ConfigController
{
public:
    ConfigController(AsyncWebServer* server);
    ~ConfigController();

    void setup();
    String serialize();
    void deserialize(const String& json);
    void load();
    void save();

    const Config::DTO::Config& getConfig();
    const Config::DTO::Light& getLightConfig(size_t index);
    const Config::DTO::Switch& getSwitchConfig(size_t index);

private:
    AsyncWebServer* _server;
    Config::DTO::Config data;

    void handleGetConfig(AsyncWebServerRequest *);
};


/// INLINEs
inline const Config::DTO::Config& ConfigController::getConfig() {
    return data;
}

inline const Config::DTO::Light& ConfigController::getLightConfig(size_t index) {
    assert(index < data.hardware.lights.size());
    return data.hardware.lights[index];
}

inline const Config::DTO::Switch& ConfigController::getSwitchConfig(size_t index) {
    assert(index < data.hardware.switches.size());
    return data.hardware.switches[index];
}

#endif