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
    void addAnimation(char *data);
    void runAnimation(uint number);
    void stopAnimation();
    uint animationCount();
};
