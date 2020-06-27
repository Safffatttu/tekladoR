#pragma once
#include <ESP8266WiFi.h>

#ifndef GIT_HASH
#define GIT_HASH "1"
#endif

namespace defaults {
const char *wifi_ssid = "";
const char *wifi_password = "";
const IPAddress mqtt_host = IPAddress(192, 168, 1, 8);
const int mqtt_port = 1883;
const std::string deviceTopic = "";
const std::string updateIp = "";
const uint16_t updatePort = 8090;
const std::string updateUrl = "";
const std::string version = GIT_HASH;
} // namespace defaults