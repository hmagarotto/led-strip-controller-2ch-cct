#include "Hardware.h"
#include "../hardware/Light.h"
#include "../hardware/Light.h"

const std::array<LightBase*, 2> lights =  {
  new Light(2, true),
  new Light(2, true),
};
