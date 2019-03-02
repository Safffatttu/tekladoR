#pragma once
#include <ESP8266WiFi.h>

#ifndef GIT_HASH
#define GIT_HASH "1"
#endif

namespace defaults
{
const char *wifi_ssid = "***REMOVED***";
const char *wifi_password = "***REMOVED***";
IPAddress mqtt_host = IPAddress(192, 168, 0, 213);
int mqtt_port = 1500;
std::string deviceTopic = "hall/";
std::string updateIp = "192.168.0.213";
uint16_t updatePort = 8090;
std::string updateUrl = "/firmware.bin";
std::string version = GIT_HASH;
} // namespace defaults