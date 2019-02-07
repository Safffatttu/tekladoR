#include <Ticker.h>
#include <iopair.hpp>
#include <animation.hpp>

void Animation::nextStep()
{
    currentAnimation.stepNumber++;

    if (currentAnimation.stepNumber < currentAnimation.lenght)
    {
        if (currentAnimation.loop)
        {
            currentAnimation.stepNumber = 0;
        }
        else
        {
            Animation::animationTicker.detach();
            return;
        }
    }

    for (size_t i = 0; i < currentAnimation.size; i++)
    {
        IOPair pair = currentAnimation.pairs[i];
        bool newValue = currentAnimation.steps[currentAnimation.stepNumber][i];
        pair.changeState(newValue);
    }
}

void Animation::startAnimation()
{
    currentAnimation = *this;
    animationTicker.attach(stepTime, nextStep);
    stepNumber = 0;
}

void Animation::stop()
{
    animationTicker.detach();
}
