#include <Arduino.h>
#include <messages.hpp>
#include <io.hpp>


void setup() {
  Serial.begin(9600);
  setupIO();
  setupNetwork();
}

void loop() {
  checkInputs();
}