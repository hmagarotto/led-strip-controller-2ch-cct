#include "Serde.h"

bool convertToJson(const Config::DTO::Config& config, JsonVariant json) {
    int lightIndex = 0;
    for (auto light : config.hardware.lights) {
        switch(light.type) {
            case Config::DTO::LightType::SIMPLE:
                json["hardware"]["lights"][lightIndex]["type"] = "SIMPLE";
                break;
            case Config::DTO::LightType::DIMMABLE:
                json["hardware"]["lights"][lightIndex]["type"] = "DIMMABLE";
                break;
            case Config::DTO::LightType::CCT:
                json["hardware"]["lights"][lightIndex]["type"] = "CCT";
                break;
        }
        json["hardware"]["lights"][lightIndex]["friendlyName"] = light.friendlyName;
        json["hardware"]["lights"][lightIndex]["uniqueId"] = light.uniqueId;
        auto pinsJson = json["hardware"]["lights"][lightIndex]["pins"].to<JsonArray>();
        for (const uint8_t& pin: light.pins) {
            pinsJson.add(pin);
        }
        auto inversePolarityJson = json["hardware"]["lights"][lightIndex]["inversePolarity"].to<JsonArray>();
        for (const bool& inversePolarity: light.inversePolarity) {
            inversePolarityJson.add(inversePolarity);
        }
        switch(light.powerOnState) {
            case Config::DTO::LightPowerOnState::ON:
                json["hardware"]["lights"][lightIndex]["powerOnState"] = "ON";
                break;
            case Config::DTO::LightPowerOnState::OFF:
                json["hardware"]["lights"][lightIndex]["powerOnState"] = "OFF";
                break;
            case Config::DTO::LightPowerOnState::LAST_STATE:
                json["hardware"]["lights"][lightIndex]["powerOnState"] = "LAST_STATE";
                break;
        }
        lightIndex++;
    }
    int switchIndex = 0;
    for (auto sw : config.hardware.switches) {
        json["hardware"]["switches"][switchIndex]["pin"] = sw.pin;
        json["hardware"]["switches"][switchIndex]["inversePolarity"] = sw.inversePolarity;
        switch (sw.action) {
            case Config::DTO::SwitchAction::TOGGLE_ALL_LIGHTS:
                json["hardware"]["switches"][switchIndex]["action"] = "TOGGLE_ALL_LIGHTS";
                break;
        }
    }
    return true;
}

void convertFromJson(JsonVariantConst& json, Config::DTO::Config& config) {
    auto lights = json["hardware"]["lights"].as<JsonArrayConst>();
    config.hardware.lights.clear();
    config.hardware.lights.reserve(4);
    for (auto lightJson : lights) {
        Config::DTO::Light light;
        if (lightJson["type"]=="SIMPLE") {
            light.type = Config::DTO::LightType::SIMPLE;
        } else if (lightJson["type"]=="DIMMABLE") {
            light.type = Config::DTO::LightType::DIMMABLE;
        } else if (lightJson["type"]=="CCT") {
            light.type = Config::DTO::LightType::CCT;
        }
        light.friendlyName = lightJson["friendlyName"].as<String>();
        light.uniqueId = lightJson["uniqueId"].as<String>();
        auto pinsJsonArray = lightJson["pins"].as<JsonArrayConst>();
        light.pins.clear();
        light.pins.reserve(4);
        for (auto pinJson : pinsJsonArray) {
            light.pins.push_back(pinJson.as<uint8_t>());
        }
        light.pins.shrink_to_fit();
        light.inversePolarity.clear();
        light.inversePolarity.reserve(light.pins.size());
        auto inversePolarityJsonArray = lightJson["inversePolarity"].as<JsonArrayConst>();
        for (auto inversePolarityJson : inversePolarityJsonArray) {
            light.inversePolarity.push_back(inversePolarityJson);
        }
        light.inversePolarity.shrink_to_fit();
        if (lightJson["powerOnState"]=="ON") {
            light.powerOnState = Config::DTO::LightPowerOnState::ON;
        } else if (lightJson["powerOnState"]=="OFF") {
            light.powerOnState = Config::DTO::LightPowerOnState::OFF;
        } else if (lightJson["powerOnState"]=="LAST_STATE") {
            light.powerOnState = Config::DTO::LightPowerOnState::LAST_STATE;
        }
        config.hardware.lights.push_back(light);
    }
    config.hardware.lights.shrink_to_fit();
    auto switches = json["hardware"]["switches"].as<JsonArrayConst>();
    config.hardware.switches.clear();
    config.hardware.switches.reserve(4);
    for (auto switchJson : switches) {
        Config::DTO::Switch sw;
        sw.pin = switchJson["pin"];
        sw.inversePolarity = switchJson["inversePolarity"];
        if (switchJson["action"] == "TOGGLE_ALL_LIGHTS") {
            sw.action = Config::DTO::SwitchAction::TOGGLE_ALL_LIGHTS;
        }
        config.hardware.switches.push_back(sw);
    }
    config.hardware.switches.shrink_to_fit();
}
