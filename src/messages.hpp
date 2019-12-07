#pragma once

#include <Arduino.h>

void publishPairMqtt(std::string, bool state);
void publishMqtt(const char *topic, uint8_t qos, bool retain,
                 const char *payload);
void setupNetwork();