#include <Arduino.h>

#include "IOManager.hpp"
#include "messages.hpp"
#include "serverTask.hpp"
#include "settings.hpp"

void setup() {
    Serial.begin(921600);
    SPIFFS.begin();

    IOManager::the().setup();

    setupNetwork();
    setupServer();
}

void loop() {

    IOManager::the().loop();
    serverTask();
}