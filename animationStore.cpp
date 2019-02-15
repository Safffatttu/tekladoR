#include <animationStore.hpp>

AnimationStore *AnimationStore::instance = nullptr;

AnimationStore::AnimationStore()
{
}

void AnimationStore::runAnimation(uint number)
{
    if (number < 0 || number >= animations.size())
    {
        return;
    }

    animations[number].start();
}

void AnimationStore::addAnimation(char *data)
{
}

void AnimationStore::addAnimation(Animation newAnimation)
{
    animations.push_back(newAnimation);
}

uint AnimationStore::animationCount()
{
    return animations.size();
}

void AnimationStore::stopAnimation()
{
    Animation::stop();
}

void AnimationStore::checkTriggers()
{
    for (auto &&animation : animations)
    {
        animation.checkTriggers();
    }
}