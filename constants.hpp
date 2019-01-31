#pragma once

#include <Ticker.h>
#include <AsyncMqttClient.hpp>
#include <ESP8266WiFi.h>

#define WIFI_SSID "***REMOVED***"
#define WIFI_PASSWORD "***REMOVED***"

#define MQTT_HOST IPAddress(192, 168, 0, 213)
#define MQTT_PORT 1883

#define EXP_ADDR_1 0x20
#define EXP_ADDR_2 0x21

extern const char* deviceTopic;

extern AsyncMqttClient mqttClient;
extern Ticker mqttReconnectTimer;

extern WiFiEventHandler wifiConnectHandler;
extern WiFiEventHandler wifiDisconnectHandler;
extern Ticker wifiReconnectTimer;
