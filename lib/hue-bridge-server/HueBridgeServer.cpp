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
    rewrite->addRule("/api/{username}/lights/{lightId}", "/api/lights/id", WebRequestMethod::HTTP_GET);
    rewrite->addRule("/api/{username}/lights/{lightId}/state", "/api/lights/id/state", WebRequestMethod::HTTP_GET | WebRequestMethod::HTTP_PUT);
    rewrite->addRule("/api/{username}/lights", "/api/lights", WebRequestMethod::HTTP_GET);
    _server->addRewrite(rewrite);

    _server->on("/description.xml", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleDescription, this, std::placeholders::_1));
    _server->on("/api/lights/id/state", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleLightStateRequest, this, std::placeholders::_1));
    _server->on("/api/lights/id/state", WebRequestMethod::HTTP_PUT, std::bind(&HueBridgeServer::handleLightStateChangeRequest, this, std::placeholders::_1));
    _server->on("/api/lights/id", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleLightDeviceRequest, this, std::placeholders::_1));
    _server->on("/api/lights", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleLightDeviceListRequest, this, std::placeholders::_1));
    _server->on("/api", WebRequestMethod::HTTP_POST, std::bind(&HueBridgeServer::handleNewUser, this, std::placeholders::_1));
}

String HueBridgeServer::handleDescriptionTemplate(const String& param) {
    if (param == "name" ) {
        return this->_name;
    } else if (param == "ip" ) {
        return F("ip");
    } else if (param == "port" ) {
        return F("port");
    } else if (param == "serial" ) {
        return F("serial");
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
    const auto lightId = lightIdParam->value().toInt();
    const auto lightDevice = this->getLightDevice(lightId);
    const auto lightState = this->getLightState(lightId);
    if (!lightState || !lightDevice) {
        request->send(404, "text/plain", "Light not found");
    }

    DynamicJsonDocument doc(1024);
    doc.set(*lightDevice);
    doc["state"] = *lightState;

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
        String idStr = String(id);
        doc[idStr] = *lightDevice;
        doc[idStr]["state"] = *lightState;
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
    const auto lightId = lightIdParam->value().toInt();
    const auto lightState = this->getLightState(lightId);
    if (!lightState) {
        request->send(404, "text/plain", "Light not found");
    }

    DynamicJsonDocument doc(1024);
    doc.set(*lightState);

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void  HueBridgeServer::handleLightStateChangeRequest(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "PUT /api/lights/id/state");
}
