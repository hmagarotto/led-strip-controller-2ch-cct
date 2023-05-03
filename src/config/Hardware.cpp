#include "Hardware.h"
#include "../hardware/Light.h"
#include "../hardware/DimmableLight.h"

const std::array<LightBase*, 2> lights =  {
  new Light(2, true),
  new DimmableLight(2, true),
};
