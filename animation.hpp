#include <iopair.hpp>
#include <Ticker.h>
#include <array>

class Animation
{
  private:
    std::vector<IOPair> pairs;
    std::vector<std::vector<bool>> steps;
    bool loop;
    float stepTime;
    uint stepNumber;

    static Ticker animationTicker;
    static Animation *currentAnimation;
    static void nextStep();

  public:
    void start();
    static void stop();

    Animation(std::vector<IOPair> pairs, uint size, std::vector<std::vector<bool>> steps, uint lenght, bool loop, float stepTime)
        : pairs(pairs), steps(steps), loop(loop), stepTime(stepTime) {}
};