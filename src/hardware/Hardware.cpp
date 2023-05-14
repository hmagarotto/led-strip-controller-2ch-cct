#include <Arduino.h>
#include "Hardware.h"
#include "hardware/Light.h"
#include "hardware/DimmableLight.h"
#include "hardware/CCTLight.h"


Hardware::Hardware() {
}

Hardware::~Hardware() {
}

void Hardware::setup(const Config::DTO::Config& config) {
  lights.reserve(config.hardware.lights.size());
  switches.reserve(config.hardware.switches.size());

  for (auto light : config.hardware.lights) {
    LightBase* hardwareLight;
    switch (light.type) {
      case Config::DTO::LightType::SIMPLE:
        hardwareLight = new Light(light.pins[0], light.inversePolarity[0]);
        break;
      case Config::DTO::LightType::DIMMABLE:
        // TODO: allow to configure gamma/transitionTime
        hardwareLight = new DimmableLight(light.pins[0], light.inversePolarity[0]);
        break;
      case Config::DTO::LightType::CCT:
        const uint8_t pins[] = {light.pins[0], light.pins[1]};
        const bool inversePolarity[] = {light.inversePolarity[0], light.inversePolarity[1]};
        // TODO: allow to configure blend/gamma/transitionTime
        hardwareLight = new CCTLight(pins,inversePolarity);
        break;
    }
    lights.push_back(hardwareLight);
  }
  lights.shrink_to_fit();

  for (auto sw : config.hardware.switches) {
    // TODO: allow to configure mode/debouncing
    Switch* hardwareSw = new Switch(sw.pin, INPUT_PULLUP, sw.inversePolarity);
    switches.push_back(hardwareSw);
  }
  switches.shrink_to_fit();
}
