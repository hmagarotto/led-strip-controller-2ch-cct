#include <functional>
#include <ESP8266WiFi.h>
#include "espnow.h"
#include "SyncController.h"


#pragma pack(push)
#pragma pack(1)
typedef struct {
    uint32_t packetId;
    uint8_t sourceLightId;
    uint8_t targetLightId;
    struct {
        bool on;
        uint8_t bri;
        uint16_t hue;
        uint8_t sat;
        float xy[2];
        uint16_t ct;
    } state;
} SyncData;
#pragma pack(pop)


SyncController::SyncController(StateController& stateController, const Config::DTO::Sync& syncConfig):
    _stateController(stateController), _syncConfig(syncConfig)
{
}

SyncController::~SyncController()
{

}

void SyncController::setup() {
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(&SyncController::s_espNowRecv);
    esp_now_register_send_cb(&SyncController::s_espNowSent);
    for (auto lightSync: this->_syncConfig.lights) {
        esp_now_add_peer(lightSync.remoteMAC, ESP_NOW_ROLE_COMBO, WiFi.channel(), NULL, 0);
    }
    _stateController.subscribe(std::bind(&SyncController::lightStateChange, this, std::placeholders::_1));
}

void SyncController::espNowRecv(u8 *mac_addr, u8 *data, u8 len) {
    Serial.printf("received from: ");
    Serial.printf(MACSTR, MAC2STR(mac_addr));
    Serial.printf("\r\n");
    SyncData syncData;
    if (len != sizeof(SyncData)) {
        Serial.print("Sync: invalid len. Ignoring...");
        return;
    }
    memcpy(&syncData, data, sizeof(SyncData));
    if (lastReceivedPacketId == syncData.packetId) {
        Serial.print("Sync: duplicated packet. Ignoring...");
        return;
    }
    lastReceivedPacketId = syncData.packetId;
    LightState state = {
        on: syncData.state.on,
        bri: syncData.state.bri,
        hue: syncData.state.hue,
        sat: syncData.state.sat,
        xy: {
            syncData.state.xy[0],
            syncData.state.xy[1],
        },
        ct: syncData.state.ct,
    };
    _stateController.updateLightState(
        StateController::StateChangeSource::SYNC,
        syncData.targetLightId,
        state,
        LightStateUpdateField::on |
        LightStateUpdateField::bri |
        LightStateUpdateField::hue |
        LightStateUpdateField::sat |
        LightStateUpdateField::xy |
        LightStateUpdateField::ct
    );
}

void SyncController::espNowSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.printf("Last sent to ");
    Serial.printf(MACSTR, MAC2STR(mac_addr));
    Serial.printf(" ");
    if (sendStatus == 0){
        Serial.printf("Delivery success");
    }
    else{
        Serial.printf("Delivery fail");
    }
    Serial.printf("\r\n");
}

void SyncController::lightStateChange(const StateController::StateChangeEvent& event) {
    if (event.source == StateController::StateChangeSource::SYNC) {
        return;
    }
    for (auto lightSync : this->_syncConfig.lights) {
        if (lightSync.lightIndex != event.lightId) {
            continue;
        }
        SyncData syncData = {
            packetId: millis(),
            sourceLightId: event.lightId,
            targetLightId: lightSync.remoteLightIndex,
            state: {
                on: event.newState.on,
                bri: event.newState.bri,
                hue: event.newState.hue,
                sat: event.newState.sat,
                xy: {
                    event.newState.xy[0],
                    event.newState.xy[1]
                },
                ct: event.newState.ct,
            }
        };
        esp_now_send(lightSync.remoteMAC, (uint8_t *) &syncData, sizeof(SyncData));
        esp_now_send(lightSync.remoteMAC, (uint8_t *) &syncData, sizeof(SyncData));
    }
}
