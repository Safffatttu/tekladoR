#pragma once
#include <ESP8266WiFi.h>

#ifndef GIT_HASH
#define GIT_HASH "1"
#endif

namespace defaults
{
const char *wifi_ssid = "";
const char *wifi_password = "";
IPAddress mqtt_host = IPAddress(192, 168, 1, 8);
int mqtt_port = 1883;
std::string deviceTopic = "";
std::string updateIp = "";
uint16_t updatePort = 8090;
std::string updateUrl = "";
std::string version = GIT_HASH;
} // namespace defaults