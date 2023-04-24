#ifndef HueBridgeController_h
#define HueBridgeController_h

#include <utility>
#include <HueBridgeServer.h>
#include <ESPAsyncWebServer.h>

class HueBridgeController : public HueBridgeServer {
public:
    HueBridgeController(AsyncWebServer *server, String&& name);
    virtual ~HueBridgeController();

    virtual void setLightState(uint8_t id, LightState state);
    virtual const LightState* getLightState(uint8_t id);
    virtual const LightDevice* getLightDevice(uint8_t id);
    virtual size_t getLightDeviceNum();
};

#endif
