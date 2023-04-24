#ifndef HueBridgeServer_h
#define HueBridgeServer_h

#include <ESPAsyncWebServer.h>
#include "lights/LightDevice.h"
#include "lights/LightState.h"

class HueBridgeServer {
public:
    HueBridgeServer(AsyncWebServer *server, String&& name);
    virtual ~HueBridgeServer();
    void setup();

    virtual void setLightState(uint8_t id, LightState state) = 0;
    virtual const LightState* getLightState(uint8_t id) = 0;
    virtual const LightDevice* getLightDevice(uint8_t id) = 0;
    virtual size_t getLightDeviceNum() = 0;

private:
    HueBridgeServer();
    AsyncWebServer *_server;
    String _name;
    String _ip;
    String _mac;

    void handleDescription(AsyncWebServerRequest *);
    String handleDescriptionTemplate(const String&);
    void handleNewUser(AsyncWebServerRequest *);
    void handleLightDeviceRequest(AsyncWebServerRequest *);
    void handleLightDeviceListRequest(AsyncWebServerRequest *);
    void handleLightStateRequest(AsyncWebServerRequest *);
    void handleLightStateChangeRequest(AsyncWebServerRequest *);

};

#endif
