#ifndef LIGHTSTATESERDE_H
#define LIGHTSTATESERDE_H

#pragma once

#include "LightState.h"
#include <ArduinoJson.h>

bool convertToJson(const LightState& src, JsonVariant dst) {
    dst["on"] = src.on;
    dst["bri"] = src.bri;
    if (src.colormode && strlen(src.colormode)) {
        dst["colormode"] = src.colormode;
        if (strcmp(src.colormode, "ct") == 0) {
            dst["ct"] = src.ct;
        } else if (strcmp(src.colormode, "hs") == 0) {
            dst["hue"] = src.hue;
            dst["sat"] = src.sat;
        } else if (strcmp(src.colormode, "xy") == 0) {
            dst["xy"][0] = src.xy[0];
            dst["xy"][1] = src.xy[1];
        }
    }
    dst["alert"] = src.alert;
    dst["mode"] = src.mode;
    dst["reachable"] = src.reachable;
    return true;
}

void updateFromJson(JsonVariantConst src, LightState& dst) {
    dst.on = src["on"];
    dst.bri = src["bri"];
    // dst.colormode = src["colormode"];

    if (strcmp(dst.colormode, "ct") == 0) {
        dst.ct = src["ct"];
    } else if (strcmp(dst.colormode, "hs") == 0) {
        dst.hue = src["hue"];
        dst.sat = src["sat"];
    } else if (strcmp(dst.colormode, "xy") == 0) {
        dst.xy[0] = src["xy"][0];
        dst.xy[1] = src["xy"][1];
    }

    // dst.alert = src["alert"];
    // dst.mode = src["mode"];
    dst.reachable = src["reachable"];
}

#endif
