#ifndef SERDE_H
#define SERDE_H

#pragma once


#include <ArduinoJson.h>
#include "config/dto/Config.h"

bool convertToJson(const Config::DTO::Config& src, JsonVariant dst);
void convertFromJson(JsonVariantConst& json, Config::DTO::Config& config);

#endif