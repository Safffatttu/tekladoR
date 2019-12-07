#include <Arduino.h>
#include <animationStore.hpp>
#include <io.hpp>
#include <messages.hpp>
#include <settings.hpp>
#include <update.h>

void parseIoMessage(std::string *topic, std::string *payload) {
    if (payload->size() == 0)
        return;
    auto indexOfName = topic->find_last_of("/") + 1;
    std::string pairName = topic->substr(indexOfName);

    int newState = atoi(payload->c_str());

    for (auto &&pair : io) {
        if (pair.name == pairName) {
            pair.changeState(newState);
        }
    }
}

void updateMqttState() {
    for (auto &&pair : io) {
        pair.updateMqttState();
    }
}

void parseAnimationMessage(std::string *topic, const char *payload) {
    int animationControll = atoi(payload);
    auto positionOfNumber = topic->find_last_of("/");
    auto animationNumberString = topic->substr(positionOfNumber + 1);
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
    std::string topic =
        (Settings::getInstance()->deviceTopic).append("animation/count");
    publishMqtt(topic.c_str(), 2, true, countChar);
}

void parseMessage(std::string *topic, std::string *payload) {
    if (topic->find("animation") != std::string::npos) {
        parseAnimationMessage(topic, payload->c_str());
    } else if (topic->find("checkState") != std::string::npos) {
        updateMqttState();
    } else if (topic->find("animation/count") != std::string::npos) {
        updateAnimationCount();
    } else if (topic->find("pair/") != std::string::npos) {
        parseIoMessage(topic, payload);
    } else if (topic->find("startUpdate") != std::string::npos) {
        updateAndRebot();
    }
}