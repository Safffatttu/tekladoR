#include <animation.hpp>
#include <vector>

class AnimationStore
{
  private:
    std::vector<Animation> animations;
    static AnimationStore *instance;
    AnimationStore();

  public:
    static AnimationStore *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new AnimationStore();
        }

        return instance;
    }

    void setup();
    void addAnimations(std::vector<Animation> newAnimations);
    void runAnimation(uint number);
    void stopAnimation();
    void checkTriggers();
    uint animationCount();
};
