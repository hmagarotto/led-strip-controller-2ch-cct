#include "Hardware.h"
#include "../hardware/CCTLight.h"
#include "../hardware/DimmableLight.h"

const std::array<LightBase*, 2> lights =  {
  new CCTLight((const uint8_t[]){12,13}, (const bool[]){false,false}),
  new CCTLight((const uint8_t[]){4,14}, (const bool[]){false,false}),
};
