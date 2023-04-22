#ifndef LIGHTDEVICE_H
#define LIGHTDEVICE_H

#pragma once

#include <cstdint>

struct LightDevice
{
    const char* type;
    const char* name;
    const char* modelid;
    const char* uniqueid;
    const char* manufacturername;
    const char* productname;
    const char* swversion;

    struct SWUpdate {
        const char* noupdates;
        const char* lastinstall;
    } swupdate;

    struct Capabilities {
        bool certified;
        struct Control {
            uint16_t mindimlevel;
            uint16_t maxlumen;
            struct CT {
                uint16_t min;
                uint16_t max;
            } ct;
        } control;
        struct Streaming {
            bool renderer;
            bool proxy;
        } streaming;
    } capabilities;

    struct Config {
        const char* archetype;
        const char* function;
        const char* direction;
        struct Startup {
            const char* mode;
            bool configured;
        } startup;
    } config;
};

#endif