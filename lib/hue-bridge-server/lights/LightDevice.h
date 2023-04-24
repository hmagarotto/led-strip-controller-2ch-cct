#ifndef LIGHTDEVICE_H
#define LIGHTDEVICE_H

#pragma once

#include <cstdint>

struct LightDevice
{
    const char* type = "Color temperature light";
    const char* name;
    const char* modelid = "LTW001";
    const char* productname = "Hue A19 White Ambiance";
    const char* uniqueid;
    const char* manufacturername = "Signify Netherlands B.V.";
    const char* swversion = "1.90.1";

    struct SWUpdate {
        const char* state = "noupdates";
        const char* lastinstall = "2020-12-09T19:13:52";
    } swupdate = {};

    struct Capabilities {
        bool certified = true;
        struct Control {
            uint16_t mindimlevel = 1000;
            uint16_t maxlumen = 806;
            struct CT {
                uint16_t min = 153;
                uint16_t max = 454;
            } ct = {};
        } control = {};
        struct Streaming {
            bool renderer = false;
            bool proxy = false;
        } streaming = {};
    } capabilities = {};

    struct Config {
        const char* archetype = "classicbulb";
        const char* function = "functional";
        const char* direction = "omnidirectional";
        struct Startup {
            const char* mode = "safety";
            bool configured = true;
        } startup = {};
    } config = {};
};

#endif