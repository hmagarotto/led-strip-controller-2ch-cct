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

    /**
     * @brief WebServer routes for config API
     * @param m WiFiMode_t
     */
    void setup();

    /**
     * @brief Serialize config to JSON string
     * @return config JSON string
     */
    String serialize();

    /**
     * @brief Deserialize JSON string into config data
     * @param json JSON string
     */
    void deserialize(const String& json);

    /**
     * @brief Load config from filesystem
     */
    void load();

    /**
     * @brief Save config to filesystem
     */
    void save();

    /**
     * @brief Get config struct
     */
    const Config::DTO::Config& getConfig();

    /**
     * @brief Get light config struct
     */
    const Config::DTO::Light& getLightConfig(size_t index);

    /**
     * @brief Get switch config struct
     */
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