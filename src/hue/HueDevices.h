#ifndef HueDevices_h
#define HueDevices_h

#include <cstddef>
#include <lights/LightDevice.h>
#include <lights/LightState.h>
#include "config/dto/Config.h"

class HueDevices {
public:
  void setup(const Config::DTO::Config& config);

  uint8_t getLightNum();
  LightState getLightState(uint8_t index);
  LightState updateLightState(uint8_t index, LightState& update, uint8_t fields);
  const LightDevice& getLightDevice(uint8_t index);

private:
  typedef struct DeviceData {
    Config::DTO::LightType type;
    LightDevice device;
    LightState state;
  } TDeviceData;
  std::vector<TDeviceData> lightList;
  LightDevice createLightDevice(const Config::DTO::Light& lightConfig);
  LightState createLightState(const Config::DTO::Light& lightConfig);
};

#endif
