#pragma once
#include <ESP8266WiFi.h>

namespace defaults
{
const char *wifi_ssid = "***REMOVED***";
const char *wifi_password = "***REMOVED***";
IPAddress mqtt_host = IPAddress(192, 168, 0, 213);
int mqtt_port = 1500;
std::string deviceTopic = "hall/";
} // namespace defaults