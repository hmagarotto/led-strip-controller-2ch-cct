#include <ESPAsyncWebServer.h>

#ifndef HueBridgeServer_h
#define HueBridgeServer_h

class HueBridgeServer {
public:
    HueBridgeServer(AsyncWebServer *server, String&& name):
        _server(server),
        _name(name)
    {
    }
    void setup();

    // void onLightStateChange(uint8_t id, LightState state);
    // LightDevice[] onLightStateRequest();
    // LightDevice onLightStateRequest(uint8_t id);

private:
    HueBridgeServer();
    AsyncWebServer *_server;
    String _name;
    String _ip;
    String _mac;

    void handleDescription(AsyncWebServerRequest *);
    String handleDescriptionTemplate(const String&);
    void handleNewUser(AsyncWebServerRequest *);

};

#endif
