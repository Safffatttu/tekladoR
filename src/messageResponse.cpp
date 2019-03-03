#include <Arduino.h>
#include <io.hpp>
#include <animationStore.hpp>
#include <update.h>
#include <settings.hpp>
#include <messages.hpp>

void parseIoMessage(char *topic, char *payload)
{
    std::string payloadString = std::string(payload);
    auto indexOfName = payloadString.find_last_of("/") + 1;
    std::string pairName = payloadString.substr(indexOfName);

    int newState = atoi(payload);

    for (auto &&pair : io)
    {
        if (pair.name == pairName)
        {
            pair.changeState(newState);
        }
    }
}

void updateMqttState()
{
    for (auto &&pair : io)
    {
        pair.updateMqttState();
    }
}

void parseAnimationMessage(std::string topic, char *payload)
{
    std::size_t foundNewAnimation = topic.find("new");

    if (foundNewAnimation != std::string::npos)
    {
        AnimationStore::getInstance()->addAnimation(payload);
    }
    else
    {
        int animationControll = atoi(payload);
        auto positionOfNumber = topic.find_last_of("/");
        auto animationNumberString = topic.substr(positionOfNumber + 1);
        int animationNumber = strtoul(animationNumberString.c_str(), nullptr, 10);

        if (animationControll == 0)
        {
            AnimationStore::getInstance()->stopAnimation();
        }
        else if (animationControll == 1)
        {
            AnimationStore::getInstance()->runAnimation(animationNumber);
        }
    }
}

void updateAnimationCount()
{
    uint count = AnimationStore::getInstance()->animationCount();
    char countChar[5];
    itoa(count, countChar, 10);
    std::string topic = (Settings::getInstance()->deviceTopic).append("animation/count");
    publishMqtt(topic.c_str(), 2, true, countChar);
}

void parseMessage(char *topic, char *payload)
{
    std::string topicString = std::string(topic);
    std::size_t foundAnimation = topicString.find("animation");

    if (foundAnimation != std::string::npos)
    {
        parseAnimationMessage(topicString, payload);
    }
    else if (topicString.find("checkState") != std::string::npos)
    {
        updateMqttState();
    }
    else if (topicString.find("animation/count") != std::string::npos)
    {
        updateAnimationCount();
    }
    else if (topicString.find("pair/") != std::string::npos)
    {
        parseIoMessage(topic, payload);
    }
    else if (topicString.find("startUpdate") != std::string::npos)
    {
        updateAndRebot();
    }
}