#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#pragma once

#include <cstdint>
#include "hardware/Hardware.h"
#include "StateController.h"

class HardwareController
{
public:
    HardwareController(Hardware& hardware, StateController& stateController);
    ~HardwareController();
    void setup();
    bool run();

private:
    Hardware& _hardware;
    StateController& _stateController;
    void lightStateChange(const StateController::StateChangeEvent& event);
    void switchStateChange(uint8_t index, int state);

};

#endif