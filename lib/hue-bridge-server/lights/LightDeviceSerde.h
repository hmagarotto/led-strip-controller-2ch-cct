#ifndef LIGHTDEVICESERDE_H
#define LIGHTDEVICESERDE_H

#pragma once

#include "LightDevice.h"
#include <ArduinoJson.hpp>


bool convertToJson(const LightDevice& src, JsonVariant dst) {
    dst["type"] = src.type;
    dst["name"] = src.name;
    dst["modelid"] = src.modelid;
    dst["productname"] = src.productname;
    dst["uniqueid"] = src.uniqueid;
    dst["manufacturername"] = src.manufacturername;
    dst["swversion"] = src.swversion;

    dst["swupdate"]["state"] = src.swupdate.state;
    dst["swupdate"]["lastinstall"] = src.swupdate.lastinstall;

    dst["capabilities"]["certified"] = src.capabilities.certified;
    dst["capabilities"]["control"]["mindimlevel"] = src.capabilities.control.mindimlevel;
    dst["capabilities"]["control"]["maxlumen"] = src.capabilities.control.maxlumen;
    dst["capabilities"]["control"]["ct"]["min"] = src.capabilities.control.ct.min;
    dst["capabilities"]["control"]["ct"]["max"] = src.capabilities.control.ct.max;
    dst["capabilities"]["streaming"]["renderer"] = src.capabilities.streaming.renderer;
    dst["capabilities"]["streaming"]["proxy"] = src.capabilities.streaming.proxy;

    dst["config"]["archetype"] = src.config.archetype;
    dst["config"]["function"] = src.config.function;
    dst["config"]["direction"] = src.config.direction;
    dst["config"]["startup"]["mode"] = src.config.startup.mode;
    dst["config"]["startup"]["configured"] = src.config.startup.configured;

    return true;
}

#endif
