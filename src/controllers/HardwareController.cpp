#include "HardwareController.h"
#include "../config/Hardware.h"
#include "../hardware/LightBase.h"


HardwareController::HardwareController(StateController& stateController):
  _stateController(stateController)
{

}

HardwareController::~HardwareController()
{

}

void HardwareController::setup() {
  for (auto light : lights) {
    light->setup();
  }
  uint8_t swIndex = 0;
  for (auto sw : switches) {
    sw->setup(std::bind(&HardwareController::switchStateChange, this, swIndex++, std::placeholders::_1));
  }
  _stateController.subscribe(std::bind(&HardwareController::lightStateChange, this, std::placeholders::_1));
}

void HardwareController::lightStateChange(const StateController::StateChangeEvent& event) {
  auto light = lights[event.lightId];
  light->setState(event.newState);
}

void HardwareController::switchStateChange(uint8_t, int) {
  _stateController.toggleAllLightState(StateController::StateChangeSource::SWITCH);
}

bool HardwareController::run() {
  bool result = false;
  for (auto light : lights) {
    result = light->run() || result;
  }
  for (auto sw : switches) {
    result = sw->run() || result;
  }
  return result;
}