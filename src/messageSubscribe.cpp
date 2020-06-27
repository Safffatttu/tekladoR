#include <Arduino.h>
#include <AsyncMqttClient.hpp>

#include "IOManager.hpp"
#include "messageSubscribe.hpp"
#include "settings.hpp"

void subscribeToPairs(AsyncMqttClient *mqttClient) {
    for (auto &&pair : IOManager::the().getPairs()) {
        std::string subscribe =
            std::string(Settings::getInstance()->deviceTopic);
        subscribe = subscribe.append("pair/");
        subscribe = subscribe.append(pair.name);
        mqttClient->subscribe(subscribe.c_str(), 2);
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