#include <Arduino.h>

#include "IOManager.hpp"
#include "animationStore.hpp"
#include "messages.hpp"
#include "settings.hpp"

void parseIoMessage(const std::string &topic, const std::string &payload) {
    if (payload.size() == 0)
        return;
    int newState = atoi(payload.c_str());

    for (auto &pair : IOManager::the().getPairs()) {
        if (pair.name == topic) {
            pair.changeState(newState);
        }
    }
}

void updateMqttState() {
    for (auto &&pair : IOManager::the().getPairs()) {
        pair.updateMqttState();
    }
}

void parseAnimationMessage(const std::string &topic, const char *payload) {
    int animationControll = atoi(payload);
    auto positionOfNumber = topic.find_last_of("/");
    auto animationNumberString = topic.substr(positionOfNumber + 1);
    int animationNumber = strtoul(animationNumberString.c_str(), nullptr, 10);

    if (animationControll == 0) {
        AnimationStore::getInstance()->stopAnimation();
    } else if (animationControll == 1) {
        AnimationStore::getInstance()->runAnimation(animationNumber);
    }
}

void updateAnimationCount() {
    uint count = AnimationStore::getInstance()->animationCount();
    char countChar[5];
    itoa(count, countChar, 10);
    std::string topic = Settings::getInstance()->deviceTopic;
    topic.append("animation/count");
    publishMqtt(topic.c_str(), 2, true, countChar);
}

void parseMessage(const std::string &topic, const std::string &payload) {
    if (topic.find("animation/count") != std::string::npos) {
        updateAnimationCount();
    } else if (topic.find("animation") != std::string::npos) {
        parseAnimationMessage(topic, payload.c_str());
    } else if (topic.find("checkState") != std::string::npos) {
        updateMqttState();
    } else {
        parseIoMessage(topic, payload);
    }
}