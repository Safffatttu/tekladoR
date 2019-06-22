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

void AnimationStore::addAnimations(std::vector<Animation> newAnimations)
{
    for(auto&& newAnimation : newAnimations)
    {
        AnimationStore::getInstance()->animations.push_back(newAnimation);
    }
    
}

uint AnimationStore::animationCount()
{
    return animations.size();
}

void AnimationStore::stopAnimation()
{
    for (auto &&ani : animations)
    {
        ani.stop();
    }
}

void AnimationStore::checkTriggers()
{
    for (auto &&animation : animations)
    {
        animation.checkTriggers();
    }
}