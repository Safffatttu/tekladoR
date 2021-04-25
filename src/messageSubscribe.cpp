#include <Arduino.h>
#include <AsyncMqttClient.hpp>

#include "IOManager.hpp"
#include "messageSubscribe.hpp"
#include "settings.hpp"

void subscribeToPairs(AsyncMqttClient *mqttClient) {
	for (const auto &pair : IOManager::the().getPairs()) {
		mqttClient->subscribe(pair.name.c_str(), 2);
	}
}

void subscribeToAnimations(AsyncMqttClient *mqttClient) {
	const auto &deviceTopic = Settings::getInstance()->deviceTopic;
	auto subscribe = std::string(deviceTopic).append("/animation/#");
	mqttClient->subscribe(subscribe.c_str(), 2);
}

void subscribe(AsyncMqttClient *mqttClient) {
	subscribeToPairs(mqttClient);
	subscribeToAnimations(mqttClient);
}