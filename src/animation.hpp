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
  std::vector<std::string> outputNames;
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
  void updateMqttState();
  static void stop();

  Animation(std::vector<IOPort> triggers, std::vector<IOPort> outputs, std::vector<std::vector<std::vector<bool>>> steps, bool loop, std::vector<std::string> outputNames, float stepTime)
      : triggers(triggers), outputs(outputs), steps(steps), outputNames(outputNames), loop(loop), stepTime(stepTime)
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