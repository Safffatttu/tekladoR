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
  bool isRunning;
  float stepTime;
  uint stepNumber;

  Ticker animationTicker;
  static void nextStep(Animation* toRun);

public:
  void start();
  void checkTriggers();
  void updateMqttState();
  void stop();
  void setState(std::vector<bool> newState);
  std::vector<bool> getState();

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
    
    stepNumber = 0;
    animationState = 0;
    isRunning = false;
    animationTicker = Ticker();
  }
};