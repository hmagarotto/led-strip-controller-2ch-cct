#ifndef HueBridgeServer_h
#define HueBridgeServer_h

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SSDPDiscovery.h"
#include "lights/LightDevice.h"
#include "lights/LightState.h"

class HueBridgeServer {
public:
    HueBridgeServer(AsyncWebServer *server, String&& name);
    virtual ~HueBridgeServer();
    void setup();
    bool run();

    virtual LightState getLightState(uint8_t id) = 0;
    virtual LightState updateLightState(uint8_t id, LightState& update, uint8_t fields) = 0;
    virtual const LightDevice* getLightDevice(uint8_t id) = 0;
    virtual uint8_t getLightDeviceNum() = 0;

private:
    HueBridgeServer();
    AsyncWebServer *_server;
    SSDPDiscovery ssdpDiscovery;
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
    void handleLightStateChangeRequestBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
};

#endif
