#ifndef DevicesConfig_h
#define DevicesConfig_h

#include <cstddef>
#include <lights/LightDevice.h>
#include <lights/LightState.h>

constexpr size_t lightNum = 2;
extern const LightDevice lightDeviceList[lightNum];
extern LightState lightStateList[lightNum];

#endif
