#include <Ticker.h>
#include <animation.hpp>
#include <animationStore.hpp>
#include <iopair.hpp>
#include <messages.hpp>
#include <settings.hpp>

void Animation::nextStep(Animation* toRun) {
  if (toRun == nullptr) return;
  toRun->stepNumber++;

  // Check if last animation step -> End animation / loop
  if (toRun->stepNumber == toRun->steps[toRun->animationState].size()) {

    if (toRun->loop) {
      toRun->stepNumber = 0;
      Serial.println("End loop");
    } else { // End animation
      Serial.println("End animation");
      toRun->animationTicker.detach();
      toRun->isRunning = false;
      toRun->updateMqttState();
      AnimationStore::getInstance()->updateAnimationGroup(toRun);

      toRun->animationState++;
      // Check if last animation state -> go to begining
      if (toRun->animationState == toRun->steps.size()) {
        toRun->animationState = 0;
      }
      return;
    }
  }

  Serial.println(toRun->animationState);
  Serial.println(toRun->stepNumber);

  // Apply changes if not end
  for (size_t i = 0; i < toRun->outputs.size(); i++) {
    Serial.println("ANIMATION");
    if (toRun->steps.size() <= toRun->animationState) {
      Serial.println(toRun->animationState);
      Serial.println("ERROR1");
      return;
    }
    if (toRun->steps[toRun->animationState].size() <= toRun->stepNumber) {
      Serial.println("ERROR2");
      Serial.println(toRun->stepNumber);
      return;
    }
    if (toRun->steps[toRun->animationState][toRun->stepNumber].size() <= i) {
      Serial.println(i);
      Serial.println("ERROR3");
      return;
    }
    bool newValue =
        toRun->steps[toRun->animationState][toRun->stepNumber][i];
    toRun->outputs[i].portWrite(newValue);
  }
}

void Animation::start() {
  Serial.print("Start animation");
  isRunning = true;
  stepNumber = 0;
  animationTicker.attach(stepTime, Animation::nextStep, this);
}

void Animation::stop() {
  Serial.print("Stop animation");
  animationTicker.detach();
  isRunning = false;
}

void Animation::updateMqttState() {
  auto deviceTopic = Settings::getInstance()->deviceTopic;
  deviceTopic = deviceTopic.append("pair/");
  for (uint i = 0; i < outputNames.size(); i++) {
    auto portName = deviceTopic.append(outputNames[i]);
    bool state = steps[animationState].back()[i];
    publishPairMqtt(portName, state);
  }
}

void Animation::checkTriggers() {
  for (size_t i = 0; i < triggers.size(); i++) {
    bool newState = (bool)triggers[i].portRead();

    if (newState != inputState[i]) {
      if (firstCycle[i]) {
        firstCycle[i] = false;
        if (isRunning){
          stop();
        } else {
          start();
        }
      }
    } else {
      firstCycle[i] = true;
    }
  }
}

void Animation::setState(std::vector<bool> newState) {
  for (size_t i = 0; i < steps.size(); i++) {
    if (steps[i].back() == newState){
      animationState = i;
      return;
    }
  }
}

std::vector<bool> Animation::getState() {
  return steps[animationState].back();
}