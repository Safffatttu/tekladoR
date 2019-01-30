#include <messages.hpp>
#include <Arduino.h>
#include <outputs.hpp>
#include <inputs.hpp>

void setup() {
  Serial.begin(9600);

  setupOutputs();
  setupInputs();
  setupNetwork();
  
  pinMode(D2, OUTPUT);
}

void loop() {
  digitalWrite(D2, HIGH);
  delay(100);
  digitalWrite(D2, LOW);
  delay(100);
}