#ifndef HueBridgeController_h
#define HueBridgeController_h

#include <utility>
#include <HueBridgeServer.h>
#include <ESPAsyncWebServer.h>
#include "StateController.h"


class HueBridgeController : public HueBridgeServer {
public:
    HueBridgeController(AsyncWebServer *server, StateController& stateController, String&& name);
    virtual ~HueBridgeController();

    virtual LightState getLightState(uint8_t id);
    virtual LightState updateLightState(uint8_t id, LightState& update, uint8_t fields);
    virtual const LightDevice* getLightDevice(uint8_t id);
    virtual uint8_t getLightDeviceNum();
protected:
    StateController& _stateController;
};

inline LightState HueBridgeController::getLightState(uint8_t id) {
    return _stateController.getLightState(id);
}

inline LightState HueBridgeController::updateLightState(uint8_t id, LightState& update, uint8_t fields) {
    return _stateController.updateLightState(
        StateController::StateChangeSource::HUE_BRIDGE, id, update, fields);
}

#endif
