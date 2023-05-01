#include "Light.h"
#include "Arduino.h"

Light::Light(uint8_t pin, bool inversePolarity) :
  _pin(pin), _inversePolarity(inversePolarity)
{
}

Light::~Light()
{
}

void Light::setup() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, physicalState(false));
}

void Light::setState(const LightState& state) {
  _on = state.on;
  digitalWrite(_pin, physicalState(_on));
}

bool Light::run() {
  return false;
}
