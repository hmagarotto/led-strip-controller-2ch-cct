#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#pragma once

#include "StateController.h"

class HardwareController
{
public:
    HardwareController(StateController& stateController);
    ~HardwareController();
    void setup();

private:
    StateController& _stateController;
    void stateChange(const StateController::StateChangeEvent& event);

};

#endif