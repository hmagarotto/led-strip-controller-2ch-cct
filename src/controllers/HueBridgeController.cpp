#include "HueBridgeController.h"

HueBridgeController::HueBridgeController(AsyncWebServer *server, StateController& stateController, String&& name):
        HueBridgeServer(server, std::move(name)),
        _stateController(stateController)
{
}

HueBridgeController::~HueBridgeController() {
}

const LightDevice* HueBridgeController::getLightDevice(uint8_t id) {
    if (id >= DevicesConfig::lightNum) {
        return NULL;
    }
    return &DevicesConfig::lightDeviceList[id];
}

uint8_t HueBridgeController::getLightDeviceNum() {
    return DevicesConfig::lightNum;
}
