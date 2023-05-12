#include <Arduino.h>
#include "Hardware.h"
#include "hardware/CCTLight.h"
#include "hardware/DimmableLight.h"

const std::array<LightBase*, 2> lights =  {
  new DimmableLight(4, false),
  new DimmableLight(14, false),
};

const std::array<Switch*, 1> switches = {
  new Switch(13, INPUT_PULLUP, true),
};
