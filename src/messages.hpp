#pragma once

#include <Arduino.h>

void publishPairMqtt(const std::string &name, bool state);
void publishMqtt(const char *topic, uint8_t qos, bool retain, const char *payload);
void setupNetwork();
void messageLoop();