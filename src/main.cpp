#define SPIFFS LITTLEFS
#include <LittleFS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <AsyncElegantOTA.h>
#include <HueBridgeServer.h>

AsyncWebServer server(80);

HueBridgeServer hueServer(&server, "ATHOM Hue Bridge");


void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  analogWriteFreq(977);
  pinMode(4, OUTPUT);
  analogWrite(4, 0);
  analogWrite(4, 10);


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

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA

  hueServer.setup();

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });

  server.begin();
  Serial.println("HTTP server started");
  analogWrite(4, 0);
}

void loop() {
  delay(1000);
}
