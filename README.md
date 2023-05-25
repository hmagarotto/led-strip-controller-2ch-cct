# Led Controller for 2 CCT channels

A firmware project to control 2 CCT lights using the [Athom RGBW controller](https://www.athom.tech/blank-1/tasmota-rgbw-light-strip-controller)

## Features

- [X] WiFiManager
- [X] OTA
    - [ ] Replace AsyncElegantOTA
- [ ] WDT
- [ ] Hue discovery
    - [X] SSDP SEARCH
    - [ ] SSDP NOTIFY
    - [ ] mDNS
- [X] Hue API
- [X] Light devices
- [X] Switches
- [ ] Web interface
- [ ] MQTT events
- [ ] Multi-device sync
- [ ] Logger

## References

* Reference Documents
    * https://arduino-esp8266.readthedocs.io/en/latest/index.html
* Dependencies
    * https://github.com/me-no-dev/ESPAsyncTCP
    * ~~https://github.com/me-no-dev/ESPAsyncWebServer~~
    * https://github.com/Aircoookie/ESPAsyncWebServer
    * https://github.com/alanswx/ESPAsyncWiFiManager
    * https://github.com/ayushsharma82/AsyncElegantOTA
* Other projects for reference
    * https://github.com/diyhue/diyHue
    * https://github.com/diyhue/Lights
    * https://github.com/Aircoookie/Espalexa
    * https://github.com/vintlabs/fauxmoESP
    * https://github.com/xoseperez/espurna
    * https://github.com/arendst/Tasmota
* Tutorials
    * https://randomnerdtutorials.com/esp8266-nodemcu-ota-over-the-air-vs-code/
    * https://randomnerdtutorials.com/esp8266-nodemcu-vs-code-platformio-littlefs/
    * https://randomnerdtutorials.com/esp8266-nodemcu-wi-fi-manager-asyncwebserver/
