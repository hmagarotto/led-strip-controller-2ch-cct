#ifndef HARDWARE_H
#define HARDWARE_H

#pragma once

#include <array>
#include "config/base.h"
#include "hardware/LightBase.h"
#include "hardware/Switch.h"

extern const std::array<LightBase*, LIGHT_QTY> lights;
extern const std::array<Switch*, SWITCH_QTY> switches;

#endif