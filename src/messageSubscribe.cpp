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
    std::string subscribe = std::string(Settings::getInstance()->deviceTopic);
    subscribe.append("animation/#");
    mqttClient->subscribe(subscribe.c_str(), 2);
}

void subscribe(AsyncMqttClient *mqttClient) {
    subscribeToPairs(mqttClient);
    subscribeToAnimations(mqttClient);
}