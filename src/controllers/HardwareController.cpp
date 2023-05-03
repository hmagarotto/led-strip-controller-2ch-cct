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
  _stateController.subscribe(std::bind(&HardwareController::stateChange, this, std::placeholders::_1));
}


void HardwareController::stateChange(const StateController::StateChangeEvent& event) {
  auto light = lights[event.lightId];
  light->setState(event.newState);
}

bool HardwareController::run() {
  bool result = false;
  for (auto light : lights) {
    result = light->run() || result;
  }
  return result;
}