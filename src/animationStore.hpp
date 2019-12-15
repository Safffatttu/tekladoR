#pragma once
#include "animation.hpp"
#include <set>
#include <vector>

class AnimationStore {
  private:
    std::vector<Animation *> animations;
    std::set<std::set<Animation *>> animationGroups;
    static AnimationStore *instance;
    AnimationStore();

  public:
    static AnimationStore *getInstance() {
        if (instance == nullptr) {
            instance = new AnimationStore();
        }

        return instance;
    }

    void setup();
    void addAnimations(std::vector<Animation *> newAnimations);
    void runAnimation(uint number);
    void stopAnimation();
    void checkTriggers();
    void updateAnimationGroup(Animation *triggeringAnimation);
    void addAnimationGroup(std::set<Animation *> animationGroup);
    uint animationCount();
};
