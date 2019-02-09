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

void AnimationStore::addAnimation(std::string data){
    
}