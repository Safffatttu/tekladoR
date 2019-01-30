#pragma once

#include <Ticker.h>
#include <AsyncMqttClient.hpp>
#include <ESP8266WiFi.h>

#define WIFI_SSID "***REMOVED***"
#define WIFI_PASSWORD "***REMOVED***"

#define MQTT_HOST IPAddress(192, 168, 0, 213)
#define MQTT_PORT 1883
#define RELAYS_COUNT 1

extern const char* deviceTopic;

extern int relayPins   [RELAYS_COUNT];
extern int inputPins   [RELAYS_COUNT];
extern int states      [RELAYS_COUNT];
extern int switchStates[RELAYS_COUNT];

extern AsyncMqttClient mqttClient;
extern Ticker mqttReconnectTimer;

extern WiFiEventHandler wifiConnectHandler;
extern WiFiEventHandler wifiDisconnectHandler;
extern Ticker wifiReconnectTimer;