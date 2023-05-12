#ifndef DevicesConfig_h
#define DevicesConfig_h

#include <cstddef>
#include <lights/LightDevice.h>
#include <lights/LightState.h>
#include "config/base.h"

class DevicesConfig {
public:
  static constexpr uint8_t lightNum = LIGHT_QTY;
  static const LightDevice lightDeviceList[lightNum];

protected:
  LightState getLightState(uint8_t index);
  LightState updateLightState(uint8_t index, LightState& update, uint8_t fields);

private:
  static LightState lightStateList[lightNum];
};

#endif
