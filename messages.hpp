#pragma once

#include <AsyncMqttClient.hpp>
#include <ESP8266WiFi.h>

#define WIFI_SSID "***REMOVED***"
#define WIFI_PASSWORD "***REMOVED***"

#define MQTT_HOST IPAddress(192, 168, 0, 213)
#define MQTT_PORT 1883

extern AsyncMqttClient mqttClient; 

void connectToWifi();
void connectToMqtt();
void onWifiConnect(const WiFiEventStationModeGotIP &event);
void onWifiDisconnect(const WiFiEventStationModeDisconnected &event);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);
void publishMqtt(int number, bool state);
void setupNetwork();