#include "HueBridgeController.h"

HueBridgeController::HueBridgeController(HueDevices& hueDevices, AsyncWebServer* server, StateController& stateController, String&& name):
        HueBridgeServer(server, std::move(name)),
        _hueDevices(hueDevices),
        _stateController(stateController)
{
}

HueBridgeController::~HueBridgeController() {
}

const LightDevice* HueBridgeController::getLightDevice(uint8_t id) {
    if (id >= this->_hueDevices.getLightNum()) {
        return NULL;
    }
    return &this->_hueDevices.getLightDevice(id);
}

uint8_t HueBridgeController::getLightDeviceNum() {
    return this->_hueDevices.getLightNum();
}
