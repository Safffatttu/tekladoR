#include <Arduino.h>
#include <animationStore.hpp>
#include <io.hpp>
#include <messages.hpp>

void setup() {
    Serial.begin(9600);
    setupNetwork();
    setupIO();
    setupAnimations();
    // testOutputs();
}

void loop() {
    checkInputs();
    AnimationStore::getInstance()->checkTriggers();
}