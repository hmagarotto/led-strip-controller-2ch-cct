#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#pragma once

#include <cstdint>
#include "StateController.h"

class HardwareController
{
public:
    HardwareController(StateController& stateController);
    ~HardwareController();
    void setup();
    bool run();

private:
    StateController& _stateController;
    void lightStateChange(const StateController::StateChangeEvent& event);
    void switchStateChange(uint8_t index, int state);

};

#endif