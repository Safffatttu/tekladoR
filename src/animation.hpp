#pragma once

#include <Ticker.h>
#include <array>

#include "iopair.hpp"

class Animation {
  private:
    std::vector<IOPort> triggers;
    std::vector<bool> inputState;
    std::vector<bool> firstCycle;

    std::vector<IOPort> outputs;
    std::vector<std::vector<std::vector<bool>>> steps;
    std::vector<std::string> outputNames;
    bool loop;
    bool isRunning;
    float stepTime;
    uint stepNumber;

    Ticker animationTicker;
    static void nextStep(Animation *toRun);

  public:
    uint animationState;
    void start();
    void checkTriggers();
    void updateMqttState();
    void stop();
    void setState(std::vector<bool> newState);
    std::vector<bool> getState();

    Animation(std::vector<IOPort> triggers, std::vector<IOPort> outputs,
              std::vector<std::vector<std::vector<bool>>> steps, bool loop,
              std::vector<std::string> outputNames, float stepTime)
        : triggers(triggers), outputs(outputs), steps(steps),
          outputNames(outputNames), loop(loop), stepTime(stepTime) {
        for (auto &&trigger : triggers) {
            trigger.setup();
        }
        for (auto &&output : outputs) {
            output.setup();
        }
        for (size_t i = 0; i < triggers.size(); i++) {
            inputState.push_back((bool)triggers[i].portRead());
            firstCycle.push_back(false);
        }

        stepNumber = 0;
        animationState = 0;
        isRunning = false;
        animationTicker = Ticker();
    }
};