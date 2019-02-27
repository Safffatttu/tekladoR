#include <Ticker.h>
#include <iopair.hpp>
#include <animation.hpp>

void Animation::nextStep()
{
    currentAnimation->stepNumber++;

    if (currentAnimation->stepNumber < currentAnimation->steps.size())
    {
        if (currentAnimation->loop)
        {
            currentAnimation->stepNumber = 0;
        }
        else
        {
            Animation::animationTicker.detach();
            return;
        }
    }

    for (size_t i = 0; i < currentAnimation->outputs.size(); i++)
    {
        IOPort port = currentAnimation->outputs[i];
        bool newValue = currentAnimation->steps[currentAnimation->stepNumber][i];
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