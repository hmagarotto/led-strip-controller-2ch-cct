#ifndef SYNCCONTROLLER_H
#define SYNCCONTROLLER_H

#pragma once

#include "config/dto/Config.h"
#include "StateController.h"

class SyncController
{
public:
    static SyncController* getSingleton(StateController& stateController, const Config::DTO::Sync& syncConfig);

    void setup();

private:
    SyncController(StateController& stateController, const Config::DTO::Sync& syncConfig);
    ~SyncController();

    void lightStateChange(const StateController::StateChangeEvent& event);
    void espNowRecv(u8 *mac_addr, u8 *data, u8 len);
    void espNowSent(u8 *mac_addr, u8 status);

    StateController& _stateController;
    const Config::DTO::Sync& _syncConfig;
    uint32_t lastReceivedPacketId;

    static SyncController* _singleton;
    static void s_espNowRecv(u8 *mac_addr, u8 *data, u8 len);
    static void s_espNowSent(u8 *mac_addr, u8 status);


};

/// inline
inline SyncController* SyncController::_singleton = nullptr;
inline SyncController* SyncController::getSingleton(StateController& stateController, const Config::DTO::Sync& syncConfig) {
    if (_singleton==nullptr) {
        _singleton = new SyncController(stateController, syncConfig);
    }
    return _singleton;
}
inline void SyncController::s_espNowRecv(u8 *mac_addr, u8 *data, u8 len) {
    _singleton->espNowRecv(mac_addr, data, len);
}
inline void SyncController::s_espNowSent(u8 *mac_addr, u8 status) {
    _singleton->espNowSent(mac_addr, status);
}


#endif