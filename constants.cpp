#include <Ticker.h>
#include <AsyncMqttClient.hpp>
#include <ESP8266WiFi.h>
#include "constants.hpp"
#include <string.h>

const char* deviceTopic = "room/";

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

Ticker wifiReconnectTimer;