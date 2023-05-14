#include <functional>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config/dto/Serde.h"
#include "ConfigController.h"


ConfigController::ConfigController(AsyncWebServer* server):
    _server(server)
{
}

ConfigController::~ConfigController()
{
}

String ConfigController::serialize() {
    DynamicJsonDocument doc(1024);
    JsonVariant root = doc.as<JsonVariant>();
    convertToJson(this->data, root);
    String output;
    serializeJson(doc, output);
    return output;
}

void ConfigController::deserialize(const String& json) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonVariantConst root = doc.as<JsonVariantConst>();
    convertFromJson(root, this->data);
}

void ConfigController::load() {
    auto file = LittleFS.open("/config.json", "r");
    if (!file) {
        return;
    }
    String json = file.readString();
    deserialize(json);
}

void ConfigController::save() {
    String json = serialize();
    auto file = LittleFS.open("/config.json", "w");
    file.print(json);
}

void ConfigController::setup() {
    _server->on("/config", WebRequestMethod::HTTP_GET, std::bind(&ConfigController::handleGetConfig, this, std::placeholders::_1));
}

void ConfigController::handleGetConfig(AsyncWebServerRequest *request) {
    String json = serialize();
    request->send(200, "application/json", json);
}
