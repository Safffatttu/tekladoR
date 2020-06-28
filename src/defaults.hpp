#pragma once
#include <ESP8266WiFi.h>

namespace defaults {
const char *wifi_ssid = "";
const char *wifi_password = "";
const IPAddress mqtt_host = IPAddress(192, 168, 1, 8);
const int mqtt_port = 1883;
const std::string deviceTopic = "";

} // namespace defaults