#include <animationStore.hpp>
#include <set>

AnimationStore *AnimationStore::instance = nullptr;

AnimationStore::AnimationStore() {}

void AnimationStore::runAnimation(uint number) {
    if (number < 0 || number >= animations.size()) {
        return;
    }

    animations[number]->start();
}

void AnimationStore::addAnimations(std::vector<Animation *> newAnimations) {
    for (auto &&newAnimation : newAnimations) {
        AnimationStore::getInstance()->animations.push_back(newAnimation);
    }
}

uint AnimationStore::animationCount() { return animations.size(); }

void AnimationStore::stopAnimation() {
    for (auto &&ani : animations) {
        ani->stop();
    }
}

void AnimationStore::checkTriggers() {
    for (auto &&animation : animations) {
        animation->checkTriggers();
    }
}

void AnimationStore::updateAnimationGroup(Animation *trigeringAnimation) {
    for (auto &&group : animationGroups) {
        for (auto &&animation : group) {
            if (animation == trigeringAnimation) {
                auto newState = trigeringAnimation->getState();
                for (auto &&animation : group) {
                    if (animation == trigeringAnimation)
                        continue;

                    animation->setState(newState);
                }
            }
        }
    }
}

void AnimationStore::addAnimationGroup(std::set<Animation *> animationGroup) {
    animationGroups.insert(animationGroup);
}