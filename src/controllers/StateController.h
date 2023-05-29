#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#pragma once

#include <circular_queue/circular_queue.h>
#include "hue/HueDevices.h"

class StateController
{
public:
    StateController(HueDevices& hueDevices);
    ~StateController();
    bool run();

    enum StateChangeSource {
        HUE_BRIDGE = 1,
        REST,
        SYNC,
        MQTT,
        SWITCH,
    };
    struct StateChangeEvent {
        StateChangeSource source;
        uint8_t lightId;
        LightState newState;
    };

    LightState getLightState(uint8_t index) {
        return _hueDevices.getLightState(index);
    }
    void toggleAllLightState(StateChangeSource changeSource);
    LightState updateLightState(StateChangeSource changeSource, uint8_t index, LightState& update, uint8_t fields);
    typedef std::function<void(const StateChangeEvent& event)> StateChangeHandlerFunction;
    void subscribe(StateChangeHandlerFunction handler);

private:
    HueDevices& _hueDevices;
    circular_queue<StateChangeEvent> _changeQueue;
    std::vector<StateChangeHandlerFunction> _changeListeners;
};

#endif
