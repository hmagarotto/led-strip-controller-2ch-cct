#define SPIFFS LITTLEFS
#include <LittleFS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <AsyncElegantOTA.h>

#include "controllers/StateController.h"
#include "controllers/HardwareController.h"
#include "controllers/HueBridgeController.h"

AsyncWebServer server(80);
StateController stateController;
HardwareController hardwareController(stateController);
HueBridgeController hueController(&server, stateController, "ATHOM Hue Bridge");


constexpr auto LED_PIN = LED_BUILTIN;
// constexpr auto LED_PIN = 4;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  analogWriteFreq(977);
  pinMode(LED_PIN, OUTPUT);
  // analogWrite(LED_PIN, 0);
  // analogWrite(LED_PIN, 40);
  digitalWrite(LED_PIN, 0);


  WiFi.mode(WIFI_STA);

  char HOSTNAME[20];
  sprintf(HOSTNAME, "ATHOM-LED-%X", ESP.getChipId());
  WiFi.setHostname(HOSTNAME);

  Serial.println("Starting WiFi");
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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response updated 3.");
  });

  hardwareController.setup();
  hueController.setup();
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
  //analogWrite(LED_PIN, 0);
  digitalWrite(LED_PIN, 1);
}

void loop() {
  static constexpr unsigned long maxDelayValue = 50;
  static unsigned long delayValue = maxDelayValue;
  bool worked = false;

  // work
  worked = stateController.run() || worked;
  yield();
  worked = hueController.run() || worked;

  // delay || yield
  if (worked) {
    delayValue = 0;
    yield();
  } else {
    if (delayValue < maxDelayValue) delayValue++;
    delay(delayValue);
  }
}
