#include <functional>
#include "HueBridgeServer.h"
#include "constants.h"
#include "PathParamsToQueryParams.h"
#include "lights/LightStateSerde.h"
#include "lights/LightDeviceSerde.h"

HueBridgeServer::HueBridgeServer(AsyncWebServer *server, String&& name):
    _server(server),
    _name(name)
{
}

HueBridgeServer::~HueBridgeServer() {
}

void HueBridgeServer::setup() {
    PathParamsToQueryParams* rewrite = new PathParamsToQueryParams();
    // rewrite->addRule("/api/{username}/lights/{lightId}", "/api/lights/id", WebRequestMethod::HTTP_GET);
    // rewrite->addRule("/api/{username}/lights/{lightId}/state", "/api/lights/id/state", WebRequestMethod::HTTP_GET | WebRequestMethod::HTTP_PUT);
    // rewrite->addRule("/api/{username}/lights", "/api/lights", WebRequestMethod::HTTP_GET);
    rewrite->addRule("/api/7fad985ede4f11edafbfb42e9910b8b3/lights/{lightId}", "/api/lights/id", WebRequestMethod::HTTP_GET);
    rewrite->addRule("/api/7fad985ede4f11edafbfb42e9910b8b3/lights/{lightId}/state", "/api/lights/id/state", WebRequestMethod::HTTP_GET | WebRequestMethod::HTTP_PUT);
    rewrite->addRule("/api/7fad985ede4f11edafbfb42e9910b8b3/lights", "/api/lights", WebRequestMethod::HTTP_GET);
    _server->addRewrite(rewrite);

    _server->on("/description.xml", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleDescription, this, std::placeholders::_1));
    _server->on("/api/lights/id/state", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleLightStateRequest, this, std::placeholders::_1));
    _server->on("/api/lights/id/state", WebRequestMethod::HTTP_PUT,
        std::bind(&HueBridgeServer::handleLightStateChangeRequest, this, std::placeholders::_1),
        NULL,
        std::bind(&HueBridgeServer::handleLightStateChangeRequestBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)
        );
    _server->on("/api/lights/id", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleLightDeviceRequest, this, std::placeholders::_1));
    _server->on("/api/lights", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleLightDeviceListRequest, this, std::placeholders::_1));
    _server->on("/api", WebRequestMethod::HTTP_POST, std::bind(&HueBridgeServer::handleNewUser, this, std::placeholders::_1));

    ssdpDiscovery.setup();
}

String HueBridgeServer::handleDescriptionTemplate(const String& param) {
    if (param == "name" ) {
        return this->_name;
    } else if (param == "ip" ) {
        IPAddress ip = WiFi.localIP();
        return ip.toString();
    } else if (param == "port" ) {
        return "80";
    } else if (param == "serial" ) {
        String mac = WiFi.macAddress();
        mac.replace(":", "");
        mac.toLowerCase();
        return mac;
    }
    return "";
}

void HueBridgeServer::handleDescription(AsyncWebServerRequest *request) {
    request->send_P(
        200,
        "text/xml",
        DESCRIPTION_XML,
        std::bind(&HueBridgeServer::handleDescriptionTemplate, this, std::placeholders::_1)
    );
}

void HueBridgeServer::handleNewUser(AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", USER_RESPONSE);
}

void HueBridgeServer::handleLightDeviceRequest(AsyncWebServerRequest *request) {
    const auto lightIdParam = request->getParam("lightId");
    if (!lightIdParam) {
        request->send(400, "text/plain", "Missing lightId param");
        return;
    }
    const auto lightId = lightIdParam->value().toInt() - 1;
    if (lightId >= getLightDeviceNum()) {
        request->send(404, "text/plain", "Light not found");
        return;
    }
    const auto lightDevice = this->getLightDevice(lightId);
    const auto lightState = this->getLightState(lightId);

    DynamicJsonDocument doc(1024);
    doc.set(*lightDevice);
    doc["state"] = lightState;

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void  HueBridgeServer::handleLightDeviceListRequest(AsyncWebServerRequest *request) {
    size_t deviceNum = this->getLightDeviceNum();
    DynamicJsonDocument doc(2048);
    for (size_t id = 0; id<deviceNum; id++) {
        const auto lightDevice = this->getLightDevice(id);
        const auto lightState = this->getLightState(id);
        String idStr = String(id + 1);
        doc[idStr] = *lightDevice;
        doc[idStr]["state"] = lightState;
    }
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void  HueBridgeServer::handleLightStateRequest(AsyncWebServerRequest *request) {
    const auto lightIdParam = request->getParam("lightId");
    if (!lightIdParam) {
        request->send(400, "text/plain", "Missing lightId param");
        return;
    }
    const auto lightId = lightIdParam->value().toInt() - 1;
    if (lightId >= getLightDeviceNum()) {
        request->send(404, "text/plain", "Light not found");
        return;
    }
    const auto lightState = this->getLightState(lightId);

    DynamicJsonDocument doc(1024);
    doc.set(lightState);

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void HueBridgeServer::handleLightStateChangeRequestBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (index == 0) {
        request->_tempObject = malloc(total+1);
        ((uint8_t*)request->_tempObject)[total] = 0;
    }
    memcpy((uint8_t*)(request->_tempObject) + index, data, len);
}

void HueBridgeServer::handleLightStateChangeRequest(AsyncWebServerRequest *request) {
    const auto lightIdParam = request->getParam("lightId");
    if (!lightIdParam) {
        request->send(400, "text/plain", "Missing lightId param");
        return;
    }
    const auto lightId = lightIdParam->value().toInt() - 1;
    if (lightId >= getLightDeviceNum()) {
        request->send(404, "text/plain", "Light not found");
        return;
    }

    DynamicJsonDocument res(256);
    LightState update;
    uint8_t fields = 0;

    {
        const char* body = (const char*)request->_tempObject;
        DynamicJsonDocument req(256);
        deserializeJson(req, body);
        JsonObject object = req.as<JsonObject>();

        auto idx = 0;
        for (JsonPair kv : object) {
            char location[32];
            sprintf(location, "/lights/%ld/state/%s", lightId+1, kv.key().c_str());
            res[idx++]["success"][location] = kv.value();

            if (kv.key() == "on") {
                update.on = kv.value();
                fields |= LightStateUpdateField::on;
            } else if (kv.key() == "bri") {
                update.bri = kv.value().as<uint8_t>();
                fields |= LightStateUpdateField::bri;
            } else if (kv.key() == "hue") {
                update.hue = kv.value().as<uint16_t>();
                fields |= LightStateUpdateField::hue;
            } else if (kv.key() == "sat") {
                update.sat = kv.value().as<uint8_t>();
                fields |= LightStateUpdateField::sat;
            } else if (kv.key() == "xy") {
                update.xy[0] = kv.value()[0].as<float>();
                update.xy[1] = kv.value()[1].as<float>();
                fields |= LightStateUpdateField::xy;
            } else if (kv.key() == "ct") {
                update.ct = kv.value().as<uint16_t>();
                fields |= LightStateUpdateField::ct;
            }
        }
    }

    this->updateLightState(lightId, update, fields);

    String output; output.reserve(256);
    serializeJson(res, output);
    request->send(200, "application/json", output);
}

bool HueBridgeServer::run() {
    return this->ssdpDiscovery.run();
}
