#include <AsyncMqttClient.hpp>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <string>

#include "messageResponse.hpp"
#include "messageSubscribe.hpp"
#include "messages.hpp"
#include "settings.hpp"

AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void connectToWifi() {
	Serial.println("Connecting to Wi-Fi...");
	const char *ssid = Settings::getInstance()->wifi_ssid.c_str();
	const char *password = Settings::getInstance()->wifi_password.c_str();
	WiFi.begin(ssid, password);
}

void connectToMqtt() {
	Serial.println("Connecting to MQTT...");
	mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP &event) {
	Serial.println("Connected to Wi-Fi.");
	Serial.println(WiFi.localIP());
	connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
	Serial.printf("Disconnected from Wi-Fi. reason: %d\n", event.reason);
	mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while
	                             // reconnecting to Wi-Fi
	wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
	Serial.println("Connected to MQTT.");
	auto devieTopic = Settings::getInstance()->deviceTopic;
	mqttClient.publish(devieTopic.append("deviceState").c_str(), 2, true, "reconnected");
	subscribe(&mqttClient);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
	Serial.println("Disconnected from MQTT.");
	Serial.printf("Mqtt client id: %s", mqttClient.getClientId());

	if (WiFi.isConnected()) {
		mqttReconnectTimer.once(2, connectToMqtt);
	}
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
	Serial.println("Subscribe acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
	Serial.print("  qos: ");
	Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
	Serial.println("Unsubscribe acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                   size_t total) {
	const auto topicString = std::string(topic);
	const auto payloadString = std::string(payload, (size_t)len);

	parseMessage(topicString, payloadString);

	Serial.println("Publish received.");
	Serial.print("  topic: ");
	Serial.println(topic);
	Serial.print("  qos: ");
	Serial.println(properties.qos);
	Serial.print("  dup: ");
	Serial.println(properties.dup);
	Serial.print("  retain: ");
	Serial.println(properties.retain);
	Serial.print("  len: ");
	Serial.println(len);
	Serial.print("  index: ");
	Serial.println(index);
	Serial.print("  total: ");
	Serial.println(total);
}

void onMqttPublish(uint16_t packetId) {
	Serial.println("Publish acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
}

void publishPairMqtt(const std::string &name, bool state) {
	char stateToPublish[2];
	itoa(static_cast<int>(state), stateToPublish, 10);

	mqttClient.publish(name.c_str(), 2, true, stateToPublish);
}

void publishMqtt(const char *topic, uint8_t qos, bool retain, const char *payload) {
	mqttClient.publish(topic, qos, retain, payload);
}

void setupNetwork() {
	wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
	wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

	WiFi.mode(WiFiMode::WIFI_AP_STA);

	// Reconfiguration acess point
	const auto reconfigureSSID = "TEKLAD_R_RECONF_" + String(spi_flash_get_id());
	const auto reconfigurePass = Settings::getInstance()->reconfigurePass;
	WiFi.softAP(reconfigureSSID.c_str(), reconfigurePass.c_str(), 1, 1);

	mqttClient.onConnect(onMqttConnect);
	mqttClient.onDisconnect(onMqttDisconnect);
	mqttClient.onSubscribe(onMqttSubscribe);
	mqttClient.onUnsubscribe(onMqttUnsubscribe);
	mqttClient.onMessage(onMqttMessage);
	mqttClient.onPublish(onMqttPublish);
	mqttClient.setServer(Settings::getInstance()->mqtt_host,
	                     Settings::getInstance()->mqtt_port);
	auto willTopic = Settings::getInstance()->deviceTopic;
	mqttClient.setWill(willTopic.append("deviceState").c_str(), 2, true, "disconnected");
	connectToWifi();
}