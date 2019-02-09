#include <iopair.hpp>
#include <Ticker.h>
#include <array>

class Animation
{
  private:
    IOPair *pairs;
    uint size;
    std::array<std::array<bool, 2>, 2> steps;
    uint lenght;
    bool loop;
    float stepTime;
    uint stepNumber;

    static Ticker animationTicker;
    static Animation *currentAnimation;
    static void nextStep();

  public:
    void start();
    static void stop();

    Animation(uint size, std::array<std::array<bool, 2>, 2> steps, uint lenght, bool loop, float stepTime)
        : size(size), steps(steps), lenght(lenght), loop(loop), stepTime(stepTime) {}
};