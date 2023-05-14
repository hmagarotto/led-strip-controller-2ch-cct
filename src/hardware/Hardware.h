#ifndef HARDWARE_H
#define HARDWARE_H

#pragma once

#include <array>
#include "config/dto/Config.h"
#include "hardware/LightBase.h"
#include "hardware/Switch.h"

class Hardware {
public:
    Hardware();
    virtual ~Hardware();

    void setup(const Config::DTO::Config& config);
    
    std::vector<LightBase*> lights;
    std::vector<Switch*> switches;
};

#endif