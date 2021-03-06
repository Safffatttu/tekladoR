#include "messageSubscribe.hpp"
#include "io.hpp"
#include "settings.hpp"
#include <Arduino.h>
#include <AsyncMqttClient.hpp>

void subscribeToPairs(AsyncMqttClient *mqttClient) {
    for (auto &&pair : io) {
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

void subscribeToStartUpdate(AsyncMqttClient *mqttClient) {
    std::string subscribe = std::string(Settings::getInstance()->deviceTopic);
    subscribe.append("startUpdate");
    mqttClient->subscribe(subscribe.c_str(), 2);
}

void subscribe(AsyncMqttClient *mqttClient) {
    subscribeToPairs(mqttClient);
    subscribeToAnimations(mqttClient);
    subscribeToStartUpdate(mqttClient);
}