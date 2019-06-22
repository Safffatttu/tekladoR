#include <Arduino.h>
#include <messages.hpp>
#include <io.hpp>
#include <animationStore.hpp>

void setup() {
  Serial.begin(9600);
  setupNetwork();
  setupIO();
  setupAnimations();
  testOutputs();
}

void loop() {
  checkInputs();
  AnimationStore::getInstance()->checkTriggers();
}