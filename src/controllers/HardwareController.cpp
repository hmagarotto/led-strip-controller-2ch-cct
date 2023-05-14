#include "HardwareController.h"
#include "hardware/LightBase.h"


HardwareController::HardwareController(Hardware& hardware, StateController& stateController):
  _hardware(hardware),
  _stateController(stateController)
{

}

HardwareController::~HardwareController()
{

}

void HardwareController::setup() {
  for (auto light : _hardware.lights) {
    light->setup();
  }
  uint8_t swIndex = 0;
  for (auto sw : _hardware.switches) {
    sw->setup(std::bind(&HardwareController::switchStateChange, this, swIndex++, std::placeholders::_1));
  }
  _stateController.subscribe(std::bind(&HardwareController::lightStateChange, this, std::placeholders::_1));
}

void HardwareController::lightStateChange(const StateController::StateChangeEvent& event) {
  auto light = _hardware.lights[event.lightId];
  light->setState(event.newState);
}

void HardwareController::switchStateChange(uint8_t index, int) {
  // TODO: use switch action config here
  _stateController.toggleAllLightState(StateController::StateChangeSource::SWITCH);
}

bool HardwareController::run() {
  bool result = false;
  for (auto light : _hardware.lights) {
    result = light->run() || result;
  }
  for (auto sw : _hardware.switches) {
    result = sw->run() || result;
  }
  return result;
}
