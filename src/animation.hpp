#pragma once
#include <iopair.hpp>
#include <Ticker.h>
#include <array>

class Animation
{
private:
  std::vector<IOPort> triggers;
  std::vector<bool> inputState;
  std::vector<bool> firstCycle;

  std::vector<IOPort> outputs;
  std::vector<std::vector<std::vector<bool>>> steps;
  uint animationState;
  bool loop;
  float stepTime;
  uint stepNumber;

  static Ticker animationTicker;
  static Animation *currentAnimation;
  static void nextStep();

public:
  void start();
  void checkTriggers();
  static void stop();

  Animation(std::vector<IOPort> triggers, std::vector<IOPort> outputs, std::vector<std::vector<std::vector<bool>>> steps, bool loop, float stepTime)
      : triggers(triggers), outputs(outputs), steps(steps), loop(loop), stepTime(stepTime)
  {
    for (auto &&trigger : triggers)
    {
      trigger.setup();
    }
    for (auto &&output : outputs)
    {
      output.setup();
    }
  }
};