#include <Ticker.h>
#include <AsyncMqttClient.hpp>
#include <ESP8266WiFi.h>
#include "constants.hpp"

const char* deviceTopic = "room/";

int relayPins   [RELAYS_COUNT] = {12};
int inputPins   [RELAYS_COUNT] = {13};
int states      [RELAYS_COUNT] = {0};
int switchStates[RELAYS_COUNT] = {0};

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

Ticker wifiReconnectTimer;