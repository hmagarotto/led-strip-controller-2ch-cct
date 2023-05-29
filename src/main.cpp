#define SPIFFS LITTLEFS
#include <LittleFS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <AsyncElegantOTA.h>

#include "config/ConfigController.h"
#include "hue/HueDevices.h"
#include "hardware/Hardware.h"
#include "controllers/StateController.h"
#include "controllers/HardwareController.h"
#include "controllers/HueBridgeController.h"
#include "controllers/SyncController.h"

AsyncWebServer server(80);
ConfigController configController(&server);
HueDevices hueDevices;
Hardware hardware;
StateController stateController(hueDevices);
SyncController* syncController = SyncController::getSingleton(stateController, configController.getConfig().sync);
HardwareController hardwareController(hardware, stateController);
HueBridgeController hueController(hueDevices, &server, stateController, "ATHOM Hue Bridge");

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  // analogWriteFreq(977);
  analogWriteFreq(1021);
  analogWriteResolution(10);

  Serial.println("Starting FS.");
  if (!LittleFS.begin()) {
    Serial.println("Failed to start FS.");
  }

  Serial.println("Loading config");
  configController.load();
  hueDevices.setup(configController.getConfig());
  hardware.setup(configController.getConfig());
  Serial.println("Config loaded");

  Serial.println("Starting WiFi");
  char HOSTNAME[20];
  sprintf(HOSTNAME, "ATHOM-LED-%X", ESP.getChipId());
  // WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setHostname(HOSTNAME);
  DNSServer dns;
  AsyncWiFiManager wifiManager(&server,&dns);
  if(!wifiManager.autoConnect(HOSTNAME)) {
        Serial.println("Failed to connect... resetting in 10 seconds");
        delay(10000);
        ESP.restart();
  }
  Serial.println("WiFi Connected");

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Expose-Headers", "*");

  configController.setup();
  hardwareController.setup();
  hueController.setup();
  syncController->setup();
  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loopDelay(bool worked) {
  static constexpr float maxDelayValue = 50;
  static constexpr float delayInc = 0.1;
  static float delayValue = maxDelayValue;

  if (worked) {
    delayValue = 1;
    return;
  }

  if (delayValue < maxDelayValue) {
    delayValue += delayInc;
  }
  delay(delayValue);
}

void loop() {
  static std::array<std::function<bool()>, 3> runners = {
    std::bind(&StateController::run, &stateController),
    std::bind(&HardwareController::run, &hardwareController),
    std::bind(&HueBridgeController::run, &hueController),
  };

  // work
  bool worked = false;
  for (auto runner : runners) {
    if (runner()) {
      yield();
      worked = true;
    }
  }

  // delay
  loopDelay(worked);
}
