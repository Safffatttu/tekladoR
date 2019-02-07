#include <ioport.hpp>
#include <Ticker.h>

class Animation
{
  private:
    IOPORT *ports;
    uint size;
    bool **steps;
    uint lenght;
    bool loop;
    float stepTime;
    uint stepNumber;

    static Ticker animationTicker;
    static Animation currentAnimation;
    static void nextStep();

  public:
    void startAnimation();

    Animation(IOPORT *ports, uint size, bool **steps, uint lenght, bool loop, float stepTime)
        : ports(ports), size(size), steps(steps), lenght(lenght), loop(loop), stepTime(stepTime) {}
};