#include <Ticker.h>
#include <animation.hpp>
#include <iopair.hpp>
#include <messages.hpp>
#include <settings.hpp>

void Animation::nextStep() {
  auto curAni = currentAnimation;
  if (curAni == nullptr)
    return;
  curAni->stepNumber++;

  // Check if last animation step -> End animation / loop
  if (curAni->stepNumber == curAni->steps[curAni->animationState].size()) {

    if (curAni->loop) {
      curAni->stepNumber = 0;
      Serial.println("End loop");
    } else { // End animation
      Serial.println("End animation");
      Animation::animationTicker.detach();
      curAni->updateMqttState();

      curAni->animationState++;
      // Check if last animation state -> go to begining
      if (curAni->animationState == curAni->steps.size()) {
        curAni->animationState = 0;
      }
      Animation::currentAnimation = nullptr;
      return;
    }
  }

  Serial.println(curAni->animationState);
  Serial.println(curAni->stepNumber);

  // Apply changes if not end
  for (size_t i = 0; i < curAni->outputs.size(); i++) {
    Serial.println("ANIMATION");
    if (curAni->steps.size() <= curAni->animationState) {
      Serial.println(curAni->animationState);
      Serial.println("ERROR1");
      return;
    }
    if (curAni->steps[curAni->animationState].size() <= curAni->stepNumber) {
      Serial.println(curAni->stepNumber);
      Serial.println("ERROR2");
      return;
    }
    if (curAni->steps[curAni->animationState][curAni->stepNumber].size() <= i) {
      Serial.println(i);
      Serial.println("ERROR3");
      return;
    }
    bool newValue =
        curAni->steps[curAni->animationState][curAni->stepNumber][i];
    curAni->outputs[i].portWrite(newValue);
  }
}

Ticker Animation::animationTicker = Ticker();
Animation *Animation::currentAnimation = nullptr;

void Animation::start() {
  Serial.print("Start animation");
  Animation::currentAnimation = this;
  stepNumber = 0;
  Animation::animationTicker.attach(stepTime, Animation::nextStep);
}

void Animation::stop() {
  Serial.print("Stop animation");
  Animation::currentAnimation = nullptr;
  animationTicker.detach();
}

void Animation::updateMqttState() {
  auto deviceTopic = Settings::getInstance()->deviceTopic;
  deviceTopic = deviceTopic.append("pair/");
  for (uint i = 0; i < currentAnimation->outputNames.size(); i++) {
    auto portName = deviceTopic.append(currentAnimation->outputNames[i]);
    bool state = currentAnimation->steps[animationState].back()[i];
    publishPairMqtt(portName, state);
  }
}

void Animation::checkTriggers() {
  for (size_t i = 0; i < triggers.size(); i++) {
    bool newState = (bool)triggers[i].portRead();

    if (newState != inputState[i]) {
      if (firstCycle[i]) {
        firstCycle[i] = false;
        if (Animation::currentAnimation == this) {
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
