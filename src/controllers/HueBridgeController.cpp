#include "HueBridgeController.h"

#include "DevicesConfig.h"

HueBridgeController::HueBridgeController(AsyncWebServer *server, String&& name):
        HueBridgeServer(server, std::move(name))
{
}

HueBridgeController::~HueBridgeController() {

}

void HueBridgeController::setLightState(uint8_t id, LightState state) {
    if (id >= lightNum) {
        return;
    }
    lightStateList[id] = state;
}

const LightState* HueBridgeController::getLightState(uint8_t id) {
    if (id >= lightNum) {
        return NULL;
    }
    return &lightStateList[id];
    
}

const LightDevice* HueBridgeController::getLightDevice(uint8_t id) {
    if (id >= lightNum) {
        return NULL;
    }
    return &lightDeviceList[id];
}

size_t HueBridgeController::getLightDeviceNum() {
    return lightNum;
}
