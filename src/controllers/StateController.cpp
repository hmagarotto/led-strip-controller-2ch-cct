#include "StateController.h"

StateController::StateController() :
  _changeQueue(32)
{
  _changeListeners.reserve(10);
}

StateController::~StateController()
{
}

void StateController::toggleAllLightState(StateChangeSource changeSource) {
  auto hasLightOn = false;
  for (size_t index = 0; index<DevicesConfig::lightNum; index++) {
    auto state=DevicesConfig::getLightState(index);
    hasLightOn = state.on || hasLightOn;
  }
  LightState update = { on: !hasLightOn };
  uint8_t fields = LightStateUpdateField::on;
  for (size_t index = 0; index<DevicesConfig::lightNum; index++) {
    updateLightState(changeSource, index, update, fields);
  }
}

LightState StateController::updateLightState(StateChangeSource changeSource, uint8_t index, LightState& update, uint8_t fields) {
  auto newState = DevicesConfig::updateLightState(index, update, fields);
  while(!_changeQueue.push({changeSource, index, newState })) {
    // ensure enqueuing of last status, even if we miss some changes
    _changeQueue.pop();
  }
  return newState;
}

void StateController::subscribe(StateChangeHandlerFunction handler) {
  _changeListeners.push_back(handler);
}

bool StateController::run() {
  bool worked = false;
  while (_changeQueue.available()) {
    worked = true;
    auto event = _changeQueue.pop();
    for (auto listener : _changeListeners) {
      listener(event);
    }
  }
  return worked;
}
