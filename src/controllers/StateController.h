#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#pragma once

#include <circular_queue/circular_queue.h>
#include "config/DevicesConfig.h"

class StateController: public DevicesConfig
{
public:
    StateController();
    ~StateController();
    bool run();

    enum StateChangeSource {
        HUE_BRIDGE = 1,
        REST,
        UDP,
        MQTT,
    };
    struct StateChangeEvent {
        StateChangeSource source;
        uint8_t lightId;
        LightState newState;
    };

    LightState getLightState(uint8_t index) {
        return DevicesConfig::getLightState(index);
    }
    LightState updateLightState(StateChangeSource changeSource, uint8_t index, LightState& update, uint8_t fields);
    typedef std::function<void(const StateChangeEvent& event)> StateChangeHandlerFunction;
    void subscribe(StateChangeHandlerFunction handler);

private:
    circular_queue<StateChangeEvent> _changeQueue;
    std::vector<StateChangeHandlerFunction> _changeListeners;
};

#endif