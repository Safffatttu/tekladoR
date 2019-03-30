#include <Ticker.h>
#include <iopair.hpp>
#include <animation.hpp>
#include <messages.hpp>
#include <settings.hpp>

void Animation::nextStep()
{
    currentAnimation->stepNumber++;

    if (currentAnimation->stepNumber < currentAnimation->steps[currentAnimation->animationState].size())
    {
        if (currentAnimation->loop)
        {
            currentAnimation->stepNumber = 0;
        }
        else
        {
            Animation::animationTicker.detach();
            currentAnimation->updateMqttState();

            if (currentAnimation->animationState < currentAnimation->steps.size())
            {
                currentAnimation->animationState++;
            }
            else 
            {
                currentAnimation->animationState = 0;
            }
            
            return;
        }
    }

    for (size_t i = 0; i < currentAnimation->outputs.size(); i++)
    {
        IOPort port = currentAnimation->outputs[i];
        bool newValue = currentAnimation->steps[currentAnimation->animationState][currentAnimation->stepNumber][i];
        port.portWrite(newValue);
    }
}

Ticker Animation::animationTicker = Ticker();
Animation *Animation::currentAnimation = nullptr;

void Animation::start()
{
    Animation::currentAnimation = this;
    Animation::animationTicker.attach(stepTime, Animation::nextStep);
    stepNumber = 0;
}

void Animation::stop()
{
    animationTicker.detach();
}

void Animation::updateMqttState()
{
    auto deviceTopic = Settings::getInstance()->deviceTopic;
    deviceTopic = deviceTopic.append("pair/");
    for(uint i = 0; i < currentAnimation->outputNames.size(); i++)
    {
        auto portName = deviceTopic.append(currentAnimation->outputNames[i]);
        bool state = currentAnimation->steps[animationState].back()[i];
        publishMqtt(portName, state);
    }
}

void Animation::checkTriggers()
{
    for (size_t i = 0; i < triggers.size(); i++)
    {
        bool newState = (bool)triggers[i].portRead();

        if (newState != inputState[i])
        {
            if (firstCycle[i])
            {
                firstCycle[i] = false;
                if (Animation::currentAnimation == this)
                {
                    stop();
                }
                else
                {
                    start();
                }
            }
        }
        else
        {
            firstCycle[i] = true;
        }
    }
}
