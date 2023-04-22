#include <functional>
#include "HueBridgeServer.h"
#include "constants.h"

void HueBridgeServer::setup() {
    _server->on("/description.xml", WebRequestMethod::HTTP_GET, std::bind(&HueBridgeServer::handleDescription, this, std::placeholders::_1));
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
