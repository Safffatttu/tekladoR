#include <Arduino.h>
#include <animationStore.hpp>
#include <io.hpp>
#include <messages.hpp>
#include <updateServer.hpp>

void setup() {
    Serial.begin(9600);
    setupNetwork();
    setupIO();
    setupAnimations();
    startUpdateServer();
    // testOutputs();
}

void loop() {
    checkInputs();
    AnimationStore::getInstance()->checkTriggers();
    serverTask();
}