#ifndef SSDPDISCOVERY_H
#define SSDPDISCOVERY_H

#pragma once

#include <Arduino.h>
#include <ESPAsyncUDP.h>
#include <circular_queue/circular_queue.h>

class SSDPDiscovery
{
public:
    SSDPDiscovery();
    ~SSDPDiscovery();

    bool setup();
    bool run();

private:
    void onUDPPacket(AsyncUDPPacket packet);

    struct SearchResponse {
        IPAddress remoteIP;
        uint16_t remotePort;
    };
    AsyncUDP udp;
    circular_queue<SearchResponse> responseQueue;
};

#endif