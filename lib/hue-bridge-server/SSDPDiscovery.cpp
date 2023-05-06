
#include <ESP8266WiFi.h>
#include "SSDPDiscovery.h"
#include "constants.h"

SSDPDiscovery::SSDPDiscovery():
    responseQueue(24)
{
}

SSDPDiscovery::~SSDPDiscovery()
{

}

bool SSDPDiscovery::setup() {
    if(!udp.listenMulticast(IPAddress(239,255,255,250), 1900)) {
        return false;
    }
    udp.onPacket(std::bind(&SSDPDiscovery::onUDPPacket, this, std::placeholders::_1));
    return true;
}

void SSDPDiscovery::onUDPPacket(AsyncUDPPacket packet) {
    String data = (const char *) packet.data();
    if (data.indexOf("M-SEARCH") < 0) {
        return;
    }
    if ((data.indexOf("ssdp:discover") < 0) && (data.indexOf("upnp:rootdevice") < 0) && (data.indexOf("device:basic:1") < 0)) {
        return;
    }

    SearchResponse response {
        packet.remoteIP(),
        packet.remotePort(),
    };
    if (!this->responseQueue.push(response)) {
        Serial.println("Warning: SSDP response queue full. Discarding response.");
    }
}

bool SSDPDiscovery::run() {
    static char body[300];
    bool worked = false;

    while (this->responseQueue.available()) {
        worked = true;
        const auto response = this->responseQueue.pop();
        
        // from fauxmoESP.cpp
        IPAddress ip = WiFi.localIP();
        String mac = WiFi.macAddress();
        mac.replace(":", "");
        mac.toLowerCase();
        
        snprintf_P(
            body, sizeof(body),
            UDP_SEARCH_RESPONSE,
            ip[0], ip[1], ip[2], ip[3],
            80,
            mac.c_str(), mac.c_str()
        );
        for (auto i = 0; i < 2; i++) {
            if(udp.writeTo((const uint8_t*)body, strlen(body), response.remoteIP, response.remotePort) > 0) {
                // Serial.printf("SSDP response sent to %s:%d\r\n", response.remoteIP.toString().c_str(), response.remotePort);
            } else {
                Serial.printf("SSDP response failed\r\n");
            }
        }
    }

    return worked;
}
