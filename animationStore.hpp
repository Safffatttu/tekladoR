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
    void addAnimation(std::string data);
    void runAnimation(uint number);
};
